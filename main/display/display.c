/**
 ******************************************************************************
 *							USEFUL ELECTRONICS
 ******************************************************************************/
/**
 ******************************************************************************
 * @file    :  display.c
 * @author  :  WARD ALMASARANI
 * @version :  v.1.0
 * @date    :  Feb 1, 2023
 * @link    :  https://www.youtube.com/@usefulelectronics
 *			   Hold Ctrl button and click on the link to be directed to
			   Useful Electronics YouTube channel	
 ******************************************************************************/


/* INCLUDES ------------------------------------------------------------------*/
#include "display.h"


/* PRIVATE STRUCTRES ---------------------------------------------------------*/

/* VARIABLES -----------------------------------------------------------------*/
lv_disp_drv_t disp_drv;  // contains callback functions
/* DEFINITIONS ---------------------------------------------------------------*/

/* MACROS --------------------------------------------------------------------*/
static const char *TAG = "forgeui_display";

static void forgeui_display_test(lv_disp_t *disp)
{
    lv_obj_t *screen = lv_disp_get_scr_act(disp);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x07121F), 0);

    lv_obj_t *ring = lv_arc_create(screen);
    lv_obj_set_size(ring, 224, 224);
    lv_obj_center(ring);
    lv_arc_set_range(ring, 0, 100);
    lv_arc_set_value(ring, 76);
    lv_arc_set_rotation(ring, 135);
    lv_obj_remove_style(ring, NULL, LV_PART_KNOB);
    lv_obj_set_style_arc_width(ring, 8, LV_PART_MAIN);
    lv_obj_set_style_arc_width(ring, 8, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ring, lv_color_hex(0x1E3850), LV_PART_MAIN);
    lv_obj_set_style_arc_color(ring, lv_color_hex(0x1DE9B6), LV_PART_INDICATOR);

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "FORGEUI");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 0, -54);

    const char *labels[] = {"GC9A01", "240 x 240", "DISPLAY TEST"};
    const lv_color_t colors[] = {
        LV_COLOR_MAKE(0x1D, 0xE9, 0xB6),
        LV_COLOR_MAKE(0xFF, 0xB7, 0x4D),
        LV_COLOR_MAKE(0x90, 0xCA, 0xF9),
    };
    const int offsets[] = {-22, 4, 32};
    for (size_t i = 0; i < 3; ++i) {
        lv_obj_t *label = lv_label_create(screen);
        lv_label_set_text(label, labels[i]);
        lv_obj_set_style_text_color(label, colors[i], 0);
        lv_obj_align(label, LV_ALIGN_CENTER, 0, offsets[i]);
    }

    const lv_color_t swatch_colors[] = {
        LV_COLOR_MAKE(0xFF, 0x17, 0x44),
        LV_COLOR_MAKE(0x00, 0xE6, 0x76),
        LV_COLOR_MAKE(0x29, 0x79, 0xFF),
    };
    const int swatch_offsets[] = {-28, 0, 28};
    for (size_t i = 0; i < 3; ++i) {
        lv_obj_t *swatch = lv_obj_create(screen);
        lv_obj_set_size(swatch, 14, 14);
        lv_obj_set_style_radius(swatch, LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(swatch, swatch_colors[i], 0);
        lv_obj_set_style_border_width(swatch, 0, 0);
        lv_obj_align(swatch, LV_ALIGN_CENTER, swatch_offsets[i], 62);
    }
}
/* FUNCTION PROTOTYPES -------------------------------------------------------*/


bool display_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_disp_drv_t *disp_driver = (lv_disp_drv_t *)user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

static void example_lvgl_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
}

/* Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. */
static void example_lvgl_port_update_callback(lv_disp_drv_t *drv)
{
    esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t) drv->user_data;

    switch (drv->rotated) {
    case LV_DISP_ROT_NONE:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, true, false);

        break;
    case LV_DISP_ROT_90:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, true, true);

        break;
    case LV_DISP_ROT_180:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, false, true);

        break;
    case LV_DISP_ROT_270:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, false, false);

        break;
    }
}



static void example_increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}


void displayConfig(void)
{
    static lv_disp_draw_buf_t disp_buf; // contains internal graphic buffer(s) called draw buffer(s)




    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();
    // alloc draw buffers used by LVGL
    // it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    lv_color_t *buf1 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    lv_color_t *buf2 = heap_caps_malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);
    // initialize LVGL draw buffers
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, EXAMPLE_LCD_H_RES * 20);

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EXAMPLE_LCD_H_RES;
    disp_drv.ver_res = EXAMPLE_LCD_V_RES;
    disp_drv.flush_cb = example_lvgl_flush_cb;
    disp_drv.drv_update_cb = example_lvgl_port_update_callback;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = panel_handle;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &example_increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));


    ESP_LOGI(TAG, "Render ForgeUI GC9A01 display test");
    forgeui_display_test(disp);

}
/*************************************** USEFUL ELECTRONICS*****END OF FILE****/
