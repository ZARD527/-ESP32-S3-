from pathlib import Path
import re

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[1]
OUT_C = ROOT / "main" / "assets" / "fonts" / "app_cjk_font.c"
OUT_H = ROOT / "main" / "assets" / "fonts" / "app_cjk_font.h"
FONT_PATH = Path("C:/Windows/Fonts/simkai.ttf")

EXTRA_CHARS = (
    "全反射信息屏天气环境日期同步便签置顶采购清单确认幕排线模块电池座和外壳螺丝"
    "固件设计页面只负责显示传感器都从服务层进入桌面提醒后续可以长按把它定到首页小栏"
    "点亮横屏界面确定驱动芯片测试校准教师节国庆中秋日历工具电量已本地关闭暂无待办"
    "网络最高最低室温湿度光照组件开始停止分计时复位投掷倒默认左侧朝下立即启动秒表骰子设备闹钟"
    "多云年月周概览待机信号今日事项室内舒适空气轻触查看详情晴雨雪雾风优良一般"
    "，。%、：｜"
)


def collect_chars():
    chars = set(EXTRA_CHARS)
    for path in (ROOT / "main").rglob("*.[ch]"):
        text = path.read_text(encoding="utf-8", errors="ignore")
        chars.update(re.findall(r"[\u4e00-\u9fff，。%、：｜]", text))
    return "".join(sorted(chars))


CHARS = collect_chars()


def pack_4bpp(img):
    pixels = img.load()
    data = []
    for y in range(img.height):
        pending = None
        for x in range(img.width):
            v = pixels[x, y] >> 4
            if pending is None:
                pending = v << 4
            else:
                data.append(pending | v)
                pending = None
        if pending is not None:
            data.append(pending)
    return data


def render_glyph(font, ch):
    left, top, right, bottom = font.getbbox(ch, anchor="ls")
    width = max(0, right - left)
    height = max(0, bottom - top)
    advance = int(round(font.getlength(ch)))
    if width == 0 or height == 0:
        return {
            "bitmap": [],
            "adv": max(advance, 1),
            "box_w": 0,
            "box_h": 0,
            "ofs_x": 0,
            "ofs_y": 0,
        }

    img = Image.new("L", (width, height), 0)
    draw = ImageDraw.Draw(img)
    draw.text((-left, -top), ch, font=font, fill=255, anchor="ls")
    return {
        "bitmap": pack_4bpp(img),
        "adv": max(advance, width),
        "box_w": width,
        "box_h": height,
        "ofs_x": left,
        "ofs_y": -bottom,
    }


def c_array_u8(name, values):
    lines = [f"static const uint8_t {name}[] = {{"]
    for i in range(0, len(values), 12):
        lines.append("    " + ", ".join(f"0x{v:02x}" for v in values[i:i + 12]) + ",")
    lines.append("};")
    return "\n".join(lines)


def c_array_u16(name, values):
    lines = [f"static const uint16_t {name}[] = {{"]
    for i in range(0, len(values), 10):
        lines.append("    " + ", ".join(f"0x{v:04x}" for v in values[i:i + 10]) + ",")
    lines.append("};")
    return "\n".join(lines)


def make_font(symbol, px, fallback):
    font = ImageFont.truetype(str(FONT_PATH), px)
    ascent, descent = font.getmetrics()
    glyphs = [render_glyph(font, ch) for ch in CHARS]
    bitmap = []
    dsc_lines = [
        f"static const lv_font_fmt_txt_glyph_dsc_t {symbol}_glyph_dsc[] = {{",
        "    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},",
    ]
    bitmap_index = 0
    for g in glyphs:
        dsc_lines.append(
            "    "
            f"{{.bitmap_index = {bitmap_index}, .adv_w = {g['adv'] * 16}, "
            f".box_w = {g['box_w']}, .box_h = {g['box_h']}, "
            f".ofs_x = {g['ofs_x']}, .ofs_y = {g['ofs_y']}}},"
        )
        bitmap.extend(g["bitmap"])
        bitmap_index += len(g["bitmap"])
    dsc_lines.append("};")

    cps = [ord(ch) for ch in CHARS]
    range_start = min(cps)
    range_length = max(cps) - range_start + 1
    unicode_list = [cp - range_start for cp in cps]

    return "\n\n".join([
        c_array_u8(f"{symbol}_glyph_bitmap", bitmap),
        "\n".join(dsc_lines),
        c_array_u16(f"{symbol}_unicode_list", unicode_list),
        f"""static const lv_font_fmt_txt_cmap_t {symbol}_cmaps[] = {{
    {{
        .range_start = 0x{range_start:x}, .range_length = {range_length}, .glyph_id_start = 1,
        .unicode_list = {symbol}_unicode_list, .glyph_id_ofs_list = NULL,
        .list_length = {len(unicode_list)}, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }},
}};

static lv_font_fmt_txt_dsc_t {symbol}_font_dsc = {{
    .glyph_bitmap = {symbol}_glyph_bitmap,
    .glyph_dsc = {symbol}_glyph_dsc,
    .cmaps = {symbol}_cmaps,
    .kern_dsc = NULL,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 4,
    .kern_classes = 0,
    .bitmap_format = LV_FONT_FMT_TXT_PLAIN
}};

lv_font_t {symbol} = {{
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,
    .line_height = {ascent + descent},
    .base_line = {descent},
    .subpx = LV_FONT_SUBPX_NONE,
    .kerning = LV_FONT_KERNING_NONE,
    .underline_position = -2,
    .underline_thickness = 1,
    .dsc = &{symbol}_font_dsc,
    .fallback = {fallback},
    .user_data = NULL,
}};"""
    ])


def main():
    if not FONT_PATH.exists():
        raise SystemExit(f"font not found: {FONT_PATH}")

    OUT_H.write_text(
        """#ifndef APP_CJK_FONT_H
#define APP_CJK_FONT_H

#include "lvgl.h"

extern lv_font_t app_cjk_font_16;
extern lv_font_t app_cjk_font_22;

#endif
""",
        encoding="utf-8",
    )

    OUT_C.write_text(
        """#include "app_cjk_font.h"
#include "src/font/lv_font_fmt_txt.h"

"""
        + make_font("app_cjk_font_16", 16, "&lv_font_montserrat_14")
        + "\n\n"
        + make_font("app_cjk_font_22", 22, "&lv_font_montserrat_20"),
        encoding="utf-8",
    )


if __name__ == "__main__":
    main()
