#include "display.h"

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_timer.h"

static const char *TAG = "forgeui_display";
lv_disp_drv_t disp_drv;

static volatile uint32_t flush_count;
static volatile uint32_t flush_ready_count;

bool display_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io,
                                     esp_lcd_panel_io_event_data_t *edata,
                                     void *user_ctx)
{
    flush_ready_count++;
    lv_disp_flush_ready((lv_disp_drv_t *)user_ctx);
    return false;
}

static void lvgl_flush_cb(lv_disp_drv_t *driver, const lv_area_t *area, lv_color_t *color_map)
{
    flush_count++;
    if (flush_count <= 3) {
        ESP_LOGI(TAG, "Flush %lu: (%d,%d)-(%d,%d), buffer=%p",
                 (unsigned long)flush_count, area->x1, area->y1, area->x2, area->y2, color_map);
    }

    esp_lcd_panel_handle_t panel = (esp_lcd_panel_handle_t)driver->user_data;
    ESP_ERROR_CHECK(esp_lcd_panel_draw_bitmap(panel, area->x1, area->y1,
                                               area->x2 + 1, area->y2 + 1, color_map));
}

static void lvgl_tick_cb(void *arg)
{
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

static void lvgl_task(void *arg)
{
    uint32_t iterations = 0;
    ESP_LOGI(TAG, "LVGL handler task started");
    for (;;) {
        lv_timer_handler();
        if (++iterations % 200 == 0) {
            ESP_LOGI(TAG, "LVGL handler alive; flush=%lu ready=%lu",
                     (unsigned long)flush_count, (unsigned long)flush_ready_count);
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

static void set_ring_value(void *ring, int32_t value)
{
    lv_arc_set_value(ring, value);
}

static void create_alive_showcase(void)
{
    lv_obj_t *screen = lv_scr_act();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x07111F), 0);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, 0);

    lv_obj_t *ring = lv_arc_create(screen);
    lv_obj_set_size(ring, 156, 156);
    lv_obj_center(ring);
    lv_arc_set_rotation(ring, 270);
    lv_arc_set_bg_angles(ring, 0, 360);
    lv_arc_set_range(ring, 0, 100);
    lv_arc_set_value(ring, 76);
    lv_obj_remove_style(ring, NULL, LV_PART_KNOB);
    lv_obj_clear_flag(ring, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_arc_width(ring, 7, LV_PART_MAIN);
    lv_obj_set_style_arc_color(ring, lv_color_hex(0x18324D), LV_PART_MAIN);
    lv_obj_set_style_arc_width(ring, 7, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(ring, lv_color_hex(0x21D4C2), LV_PART_INDICATOR);

    lv_obj_t *brand = lv_label_create(screen);
    lv_label_set_text(brand, "FORGEUI");
    lv_obj_set_style_text_color(brand, lv_color_hex(0xF4FAFF), 0);
    lv_obj_set_style_text_font(brand, &lv_font_montserrat_14, 0);
    lv_obj_align(brand, LV_ALIGN_TOP_MID, 0, 25);

    lv_obj_t *lab = lv_label_create(screen);
    lv_label_set_text(lab, "HARDWARE LAB");
    lv_obj_set_style_text_color(lab, lv_color_hex(0x75A7C7), 0);
    lv_obj_set_style_text_font(lab, &lv_font_montserrat_14, 0);
    lv_obj_align(lab, LV_ALIGN_TOP_MID, 0, 51);

    lv_obj_t *ready = lv_label_create(screen);
    lv_label_set_text(ready, "DISPLAY READY");
    lv_obj_set_style_text_color(ready, lv_color_hex(0x21D4C2), 0);
    lv_obj_set_style_text_font(ready, &lv_font_montserrat_14, 0);
    lv_obj_center(ready);

    lv_obj_t *spec = lv_label_create(screen);
    lv_label_set_text(spec, "GC9A01  |  240 x 240\nESP32-S3");
    lv_obj_set_style_text_color(spec, lv_color_hex(0xA9C2D4), 0);
    lv_obj_set_style_text_align(spec, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(spec, &lv_font_montserrat_14, 0);
    lv_obj_align(spec, LV_ALIGN_BOTTOM_MID, 0, -28);

    lv_anim_t animation;
    lv_anim_init(&animation);
    lv_anim_set_var(&animation, ring);
    lv_anim_set_values(&animation, 12, 96);
    lv_anim_set_time(&animation, 1800);
    lv_anim_set_playback_time(&animation, 1800);
    lv_anim_set_repeat_count(&animation, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_exec_cb(&animation, set_ring_value);
    lv_anim_start(&animation);

    lv_obj_invalidate(screen);
    ESP_LOGI(TAG, "ForgeUI GC9A01 alive showcase created");
}

void displayConfig(void)
{
    static lv_disp_draw_buf_t draw_buffer;
    lv_color_t *buffer_a = heap_caps_malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_color_t *buffer_b = heap_caps_malloc(EXAMPLE_LCD_H_RES * 20 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buffer_a && buffer_b);

    ESP_LOGI(TAG, "Initialize LVGL; buffers=%p,%p", buffer_a, buffer_b);
    lv_init();
    lv_disp_draw_buf_init(&draw_buffer, buffer_a, buffer_b, EXAMPLE_LCD_H_RES * 20);
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = EXAMPLE_LCD_H_RES;
    disp_drv.ver_res = EXAMPLE_LCD_V_RES;
    disp_drv.flush_cb = lvgl_flush_cb;
    disp_drv.draw_buf = &draw_buffer;
    disp_drv.user_data = panel_handle;
    lv_disp_drv_register(&disp_drv);
    ESP_LOGI(TAG, "LVGL display driver registered: %dx%d", EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);

    const esp_timer_create_args_t tick_timer_args = {
        .callback = lvgl_tick_cb,
        .name = "lvgl_tick",
    };
    esp_timer_handle_t tick_timer;
    ESP_ERROR_CHECK(esp_timer_create(&tick_timer_args, &tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));
    ESP_LOGI(TAG, "LVGL tick timer started at %d ms", EXAMPLE_LVGL_TICK_PERIOD_MS);

    create_alive_showcase();
    assert(xTaskCreate(lvgl_task, "lvgl", 4096, NULL, 4, NULL) == pdPASS);
}
