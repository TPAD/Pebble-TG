#include <pebble.h>
#define KEY_BUTTON 0
#define UPVOTE_BUTTON 1
#define REQUEST_BUTTON 2
#define DOWNVOTE_BUTTON 3

static void send(int key, int value);
static void out_sent_handler(DictionaryIterator *iterator, void *context);

int currentWindow = 0;

Window *tg_main_window;
Window *tg_upvote_window;
Window *tg_hotspots_window;
Window *tg_downvote_window;

TextLayer *tg_text_upvote;
TextLayer *tg_text_hotspots;
TextLayer *tg_text_downvote;
TextLayer *requesting;

MenuLayer *menu_layer;

BitmapLayer *thumbs_up;
BitmapLayer *thumbs_down;

GBitmap *upvoteBitmap;
GBitmap *downvoteBitmap;

void up_button(ClickRecognizerRef recognizer, void *context)
{
  if (currentWindow == 0) {
    // currently main window
    // Button pressed, tell iOS
    send(KEY_BUTTON, UPVOTE_BUTTON);
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
    send(KEY_BUTTON, REQUEST_BUTTON);
    window_stack_push(tg_hotspots_window, true);
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
    // Button pressed, tell iOS
    send(KEY_BUTTON, DOWNVOTE_BUTTON);
    window_stack_push(tg_downvote_window, true);
    currentWindow = 5;
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

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
  switch(cell_index->row)
  {
  case 0:
    menu_cell_basic_draw(ctx, cell_layer, "1. Location", "Pin Numbers", NULL);
    break;
  case 1:
    menu_cell_basic_draw(ctx, cell_layer, "2. Location", "Pin Numbers", NULL);
    break;
  case 2:
    menu_cell_basic_draw(ctx, cell_layer, "3. Location", "Pin Numbers", NULL);
    break;
  case 3:
    menu_cell_basic_draw(ctx, cell_layer, "4. Location", "Pin Numbers", NULL);
    break;
  case 4:
    menu_cell_basic_draw(ctx, cell_layer, "5. Location", "Pin Numbers", NULL);
    break;
  }
}

uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
  return 5;
}

void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{ 
  
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

void tg_hotspots_load(Window *window)
{
  //Select Button Pressed
  menu_layer = menu_layer_create(GRect(0, 0, 144, 152));
  
  menu_layer_set_click_config_onto_window(menu_layer, window);
  MenuLayerCallbacks callbacks = {
        .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
        .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
        .select_click = (MenuLayerSelectCallback) select_click_callback
  };
  menu_layer_set_callbacks(menu_layer, NULL, callbacks);

  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}

void tg_hotspots_unload(Window *window)
{
  menu_layer_destroy(menu_layer);
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
  tg_text_hotspots = text_layer_create(GRect(23, 60, 120, 32));
  text_layer_set_background_color(tg_text_hotspots, GColorBlack);
  text_layer_set_text_color(tg_text_hotspots, GColorClear);
  text_layer_set_text(tg_text_hotspots, "HOTSPOTS");
  
  text_layer_set_font(tg_text_hotspots, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_alignment(tg_text_hotspots, GTextAlignmentCenter);
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(tg_text_hotspots));
  
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
  text_layer_destroy(tg_text_hotspots);
  text_layer_destroy(tg_text_downvote);
}

void init()
{
  upvoteBitmap = gbitmap_create_with_resource(RESOURCE_ID_Thumbs_Up_WHITE);
  downvoteBitmap = gbitmap_create_with_resource(RESOURCE_ID_Thumbs_Down_WHITE_WHITE);
  tg_main_window = window_create();
  tg_upvote_window = window_create();
  tg_hotspots_window = window_create();
  tg_downvote_window = window_create();
  window_set_window_handlers(tg_main_window, (WindowHandlers) {
    .load = tg_main_load,
    .unload = tg_main_unload,
  });
  window_set_window_handlers(tg_upvote_window, (WindowHandlers) {
    .load = tg_upvote_load,
    .unload = tg_upvote_unload,
  });
  window_set_window_handlers(tg_hotspots_window, (WindowHandlers) {
    .load = tg_hotspots_load,
    .unload = tg_hotspots_unload,
  });
  window_set_window_handlers(tg_downvote_window, (WindowHandlers) {
    .load = tg_downvote_load,
    .unload = tg_downvote_unload,
  });
  window_set_click_config_provider(tg_main_window, click_config_provider);
  window_set_click_config_provider(tg_upvote_window, click_config_provider);
  window_set_click_config_provider(tg_hotspots_window, click_config_provider);
  window_set_click_config_provider(tg_downvote_window, click_config_provider);
  window_stack_push(tg_main_window, true);

  app_message_open(app_message_inbox_size_maximum(), 
  	app_message_outbox_size_maximum());
  app_message_register_outbox_sent(out_sent_handler);
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

static void send(int key, int value) {
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  dict_write_int(iter, key, &value, sizeof(int), true);

  app_message_outbox_send();
}

static void out_sent_handler(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Message was sent ok!");
}