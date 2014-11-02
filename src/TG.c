#include <pebble.h>

bool select = true;
bool up = true;
bool down = true;

Window *tg_window;

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
  layer_set_hidden((Layer *)thumbs_up, false);
  layer_set_hidden((Layer *)tg_text_upvote, true); 
  layer_set_hidden((Layer *)tg_text_downvote, true);
  layer_set_hidden((Layer *)tg_text_uber, true);
  layer_set_hidden((Layer *)requesting, true); 
  layer_set_hidden((Layer *)thumbs_down, true);
}

void select_button(ClickRecognizerRef recognizer, void *context)
{
  layer_set_hidden((Layer *)tg_text_upvote, true); 
  layer_set_hidden((Layer *)tg_text_downvote, true);
  layer_set_hidden((Layer *)tg_text_uber, true);
  layer_set_hidden((Layer *)thumbs_up, true);
  layer_set_hidden((Layer *)thumbs_down, true);
}

void down_button(ClickRecognizerRef recognizer, void *context)
{
  layer_set_hidden((Layer *)thumbs_down, false);
  layer_set_hidden((Layer *)tg_text_upvote, true); 
  layer_set_hidden((Layer *)tg_text_downvote, true);
  layer_set_hidden((Layer *)tg_text_uber, true);
  layer_set_hidden((Layer *)requesting, true);
  layer_set_hidden((Layer *)thumbs_up, true);
}
   
void click_config_provider(void *context)
{
  window_single_click_subscribe(BUTTON_ID_UP, up_button);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_button);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_button);
}

void tg_load(Window *window)
{
  //Upvote or drop pin
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
  
  //Downvote button
  tg_text_downvote = text_layer_create(GRect(63, 115, 80, 32));
  text_layer_set_background_color(tg_text_downvote, GColorBlack);
  text_layer_set_text_color(tg_text_downvote, GColorClear);
  text_layer_set_text(tg_text_downvote, "DROP IT");
  
  text_layer_set_font(tg_text_downvote, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(tg_text_downvote, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tg_text_downvote));
  
   //Select Button Pressed
  requesting = text_layer_create(GRect(10, 60, 120, 32));
  text_layer_set_background_color(requesting, GColorClear);
  text_layer_set_text_color(requesting, GColorBlack);
  text_layer_set_text(requesting, "Requesting...");
  
  text_layer_set_font(requesting, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(requesting, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tg_text_downvote));
  layer_insert_below_sibling((Layer *)requesting, (Layer *)tg_text_uber);
  
  //Up Button Pressed
  thumbs_up = bitmap_layer_create(GRect(25, 25, 100, 100));
  bitmap_layer_set_alignment(thumbs_up, GAlignCenter);
  bitmap_layer_set_background_color(thumbs_up, GColorClear);
  bitmap_layer_set_bitmap(thumbs_up, upvoteBitmap);
  bitmap_layer_set_compositing_mode	(thumbs_up, GCompOpAssign);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(thumbs_up));
  layer_insert_below_sibling((Layer *)thumbs_up, (Layer *)requesting);
  layer_set_hidden((Layer *)thumbs_up, true);
  
  
  //Down Button Pressed
  thumbs_down = bitmap_layer_create(GRect(25, 25, 100, 100));
  bitmap_layer_set_alignment(thumbs_down, GAlignCenter);
  bitmap_layer_set_background_color(thumbs_down, GColorClear);
  bitmap_layer_set_bitmap(thumbs_down, downvoteBitmap);
  bitmap_layer_set_compositing_mode	(thumbs_down, GCompOpAssign);
  
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(thumbs_down));
  layer_insert_below_sibling((Layer *)thumbs_down, (Layer *)thumbs_up);
  layer_set_hidden((Layer *)thumbs_down, true);
  
}


void tg_unload(Window *window)
{
  text_layer_destroy(tg_text_upvote);
  text_layer_destroy(tg_text_uber);
  text_layer_destroy(tg_text_downvote);
  text_layer_destroy(requesting);
  
  bitmap_layer_destroy(thumbs_up);
  bitmap_layer_destroy(thumbs_down);
}

void init()
{
  upvoteBitmap = gbitmap_create_with_resource(RESOURCE_ID_Thumbs_Up_WHITE);
  downvoteBitmap = gbitmap_create_with_resource(RESOURCE_ID_Thumbs_Down_WHITE_WHITE);
  tg_window = window_create();
  window_set_window_handlers(tg_window, (WindowHandlers) {
    .load = tg_load,
    .unload = tg_unload,
  });
  window_set_click_config_provider(tg_window, click_config_provider);
  window_stack_push(tg_window, true);
}


void deinit()
{
  window_destroy(tg_window);
}  
  
int main(void)
{
  init();
  app_event_loop();
  deinit();
}


