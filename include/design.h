#ifndef _COLLEGUI_DESIGN_H
#define _COLLEGUI_DESIGN_H

typedef struct {
    unsigned int bg_color;      // 背景色 (ARGB)
    unsigned int fg_color;      // 文字色 (ARGB)
    unsigned int ripple_color;  // リップルエフェクト色 (ARGB)
    unsigned int border_radius; // 角丸ピクセル
    unsigned int elevation;     // 影の高さ
    const char *font_family;    // フォント名
    unsigned int font_size;     // フォントサイズ
    unsigned int border_width;  // 枠線の太さ
    unsigned int border_color;  // 枠線色 (ARGB)
} MaterialButtonStyle;

typedef struct {
    unsigned int fg_color;      // 文字色 (ARGB)
    const char *font_family;    // フォント名
    unsigned int font_size;     // フォントサイズ
} MaterialLabelStyle;

typedef struct {
    unsigned int bg_color;      // 背景色 (ARGB)
    unsigned int fg_color;      // 文字色 (ARGB)
    unsigned int border_radius; // 角丸ピクセル
    unsigned int border_width;  // 枠線の太さ
    unsigned int border_color;  // 枠線色 (ARGB)
    const char *font_family;    // フォント名
    unsigned int font_size;     // フォントサイズ
} MaterialTextBoxStyle;

typedef struct {
    unsigned int box_color;     // チェックボックス枠色 (ARGB)
    unsigned int check_color;   // チェックマーク色 (ARGB)
    unsigned int bg_color;      // 背景色 (ARGB)
    unsigned int label_color;   // ラベル文字色 (ARGB)
    unsigned int border_radius; // 角丸ピクセル
    const char *font_family;    // フォント名
    unsigned int font_size;     // フォントサイズ
} MaterialCheckBoxStyle;

typedef struct {
    unsigned int bg_color;      // 背景色 (ARGB)
    unsigned int fg_color;      // 文字色 (ARGB)
    unsigned int selected_bg;   // 選択時背景色 (ARGB)
    unsigned int selected_fg;   // 選択時文字色 (ARGB)
    unsigned int border_radius; // 角丸ピクセル
    unsigned int border_width;  // 枠線の太さ
    unsigned int border_color;  // 枠線色 (ARGB)
    const char *font_family;    // フォント名
    unsigned int font_size;     // フォントサイズ
} MaterialListBoxStyle;

typedef struct {
    unsigned int bg_color;      // 背景色 (ARGB)
    unsigned int fg_color;      // 文字色 (ARGB)
    unsigned int border_radius; // 角丸ピクセル
    unsigned int elevation;     // 影の高さ
    const char *font_family;    // フォント名
    unsigned int font_size;     // フォントサイズ
} MaterialToolbarStyle;

extern const MaterialButtonStyle DefaultMaterialButtonStyle;
extern const MaterialLabelStyle DefaultMaterialLabelStyle;
extern const MaterialTextBoxStyle DefaultMaterialTextBoxStyle;
extern const MaterialCheckBoxStyle DefaultMaterialCheckBoxStyle;
extern const MaterialListBoxStyle DefaultMaterialListBoxStyle;

#endif
