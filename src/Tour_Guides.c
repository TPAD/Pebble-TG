#include <pebble.h>

int currentWindow = 0;

Window *tg_main_window;
Window *tg_upvote_window;
Window *tg_request_window;
Window *tg_downvote_window;

TextLayer *tg_text_upvote;
TextLayer *tg_text_uber;
TextLayer *tg_text_downvote;
TextLayer *requesting;

BitmapLayer *thumbs_up;
BitmapLayer *thumbs_down;

GBitmap *upvoteBitmap;
GBitmap *downvoteBitmap;

void up_button(ClickRecognizerRef recognizer, void *context)
{
  if (currentWindow == 0) {
    // currently main window
    window_stack_push(tg_upvote_window, true);
    currentWindow = 1;
  } else if (currentWindow == 1) {
    // currently upvote window 
    window_stack_pop(true);
    currentWindow = 0;
  }
}

void select_button(ClickRecognizerRef recognizer, void *context)
{
  if (currentWindow == 0) {
    // currently main window 
    window_stack_push(tg_request_window, true);
    currentWindow = 2;
  } else if (currentWindow == 2) {
    // currently request window 
    window_stack_pop(true);
    currentWindow = 0;
  }
}

void down_button(ClickRecognizerRef recognizer, void *context)
{
  if (currentWindow == 0) {
    // currently main window 
    window_stack_push(tg_downvote_window, true);
    currentWindow = 3;
  } else if (currentWindow == 3) {
    // currently request window 
    window_stack_pop(true);
    currentWindow = 0;
  }
}
   
void click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_UP, up_button);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_button);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_button);
}

void tg_upvote_load(Window *window)
{
  //Up Button Pressed
  thumbs_up = bitmap_layer_create(GRect(25, 25, 100, 100));
  bitmap_layer_set_alignment(thumbs_up, GAlignCenter);
  bitmap_layer_set_background_color(thumbs_up, GColorClear);
  bitmap_layer_set_bitmap(thumbs_up, upvoteBitmap);
  bitmap_layer_set_compositing_mode	(thumbs_up, GCompOpAssign);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(thumbs_up));
}

void tg_upvote_unload(Window *window)
{
  bitmap_layer_destroy(thumbs_up);
}

void tg_request_load(Window *window)
{
  //Select Button Pressed
  requesting = text_layer_create(GRect(10, 60, 120, 32));
  text_layer_set_background_color(requesting, GColorClear);
  text_layer_set_text_color(requesting, GColorBlack);
  text_layer_set_text(requesting, "Requesting...");
  text_layer_set_font(requesting, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(requesting, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(requesting));
}

void tg_request_unload(Window *window)
{
  text_layer_destroy(requesting);
}

void tg_downvote_load(Window *window)
{
  //Down Button Pressed
  thumbs_down = bitmap_layer_create(GRect(25, 25, 100, 100));
  bitmap_layer_set_alignment(thumbs_down, GAlignCenter);
  bitmap_layer_set_background_color(thumbs_down, GColorClear);
  bitmap_layer_set_bitmap(thumbs_down, downvoteBitmap);
  bitmap_layer_set_compositing_mode	(thumbs_down, GCompOpAssign);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(thumbs_down));
}

void tg_downvote_unload(Window *window)
{
  bitmap_layer_destroy(thumbs_down);
}

void tg_main_load(Window *window)
{
  //Drop upvote pin
  tg_text_upvote = text_layer_create(GRect(63, 5, 80, 32));
  text_layer_set_background_color(tg_text_upvote, GColorBlack);
  text_layer_set_text_color(tg_text_upvote, GColorClear);
  text_layer_set_text(tg_text_upvote, "BUMP IT");
  
  text_layer_set_font(tg_text_upvote, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(tg_text_upvote, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tg_text_upvote));
  
  //Call an Uber button
  tg_text_uber = text_layer_create(GRect(23, 60, 120, 32));
  text_layer_set_background_color(tg_text_uber, GColorBlack);
  text_layer_set_text_color(tg_text_uber, GColorClear);
  text_layer_set_text(tg_text_uber, "REQUEST UBER");
  
  text_layer_set_font(tg_text_uber, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(tg_text_uber, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tg_text_uber));
  
  //Drop downvote pin
  tg_text_downvote = text_layer_create(GRect(63, 115, 80, 32));
  text_layer_set_background_color(tg_text_downvote, GColorBlack);
  text_layer_set_text_color(tg_text_downvote, GColorClear);
  text_layer_set_text(tg_text_downvote, "DROP IT");
  
  text_layer_set_font(tg_text_downvote, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(tg_text_downvote, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tg_text_downvote));
}

void tg_main_unload(Window *window)
{
  text_layer_destroy(tg_text_upvote);
  text_layer_destroy(tg_text_uber);
  text_layer_destroy(tg_text_downvote);
}

void init()
{
  upvoteBitmap = gbitmap_create_with_resource(RESOURCE_ID_Thumbs_Up_WHITE);
  downvoteBitmap = gbitmap_create_with_resource(RESOURCE_ID_Thumbs_Up_WHITE);
  tg_main_window = window_create();
  tg_upvote_window = window_create();
  tg_request_window = window_create();
  tg_downvote_window = window_create();
  window_set_window_handlers(tg_main_window, (WindowHandlers) {
    .load = tg_main_load,
    .unload = tg_main_unload,
  });
  window_set_window_handlers(tg_upvote_window, (WindowHandlers) {
    .load = tg_upvote_load,
    .unload = tg_upvote_unload,
  });
  window_set_window_handlers(tg_request_window, (WindowHandlers) {
    .load = tg_request_load,
    .unload = tg_request_unload,
  });
  window_set_window_handlers(tg_downvote_window, (WindowHandlers) {
    .load = tg_downvote_load,
    .unload = tg_downvote_unload,
  });
  window_set_click_config_provider(tg_main_window, click_config_provider);
  window_set_click_config_provider(tg_upvote_window, click_config_provider);
  window_set_click_config_provider(tg_request_window, click_config_provider);
  window_set_click_config_provider(tg_downvote_window, click_config_provider);
  window_stack_push(tg_main_window, true);
}

void deinit()
{
  window_destroy(tg_main_window);
}  

int main(void)
{
  init();
  app_event_loop();
  deinit();
}