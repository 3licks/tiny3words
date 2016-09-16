#include <pebble.h>

Window *mainW;
TextLayer *title;
TextLayer *loc;
char *words;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;
  
  // Prepare the outbox buffer for this message
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if(result == APP_MSG_OK) {
    result = app_message_outbox_send();
  }
}

static void click_config_provider(void *context) {
  // Subcribe to button click events here
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  // Is the location name inside this message?
  Tuple *wordsTupple = dict_find(iter, MESSAGE_KEY_words);
  if(wordsTupple) {
    words = wordsTupple->value->cstring;

    text_layer_set_text(loc, words);
  }
}

void init() {
  mainW = window_create();
  window_set_click_config_provider(mainW, click_config_provider);
  
  #if defined(PBL_RECT)
    Layer *background = layer_create(GRect(0, 0, 144, 168));
  #elif defined(PBL_ROUND)
    Layer *background = layer_create(GRect(0, 0, 180, 180));
  #endif
  GRect bounds = layer_get_bounds(background);
  title = text_layer_create(bounds);
  text_layer_set_text(title, "tiny3words");
  text_layer_set_font(title, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  layer_add_child(window_get_root_layer(mainW), text_layer_get_layer(title));
  text_layer_enable_screen_text_flow_and_paging(title, 2);
  window_stack_push(mainW, true);
  TextLayer label1 = text_layer_create(bounds);
  TextLayer label2 = text_layer_create(bounds);
  loc = text_layer_create(bounds);
  text_layer_set_text(label1, "Your location:");
  text_layer_set_text(label2, "Press select to refresh:");
  text_layer_enable_screen_text_flow_and_paging(label1, 2);
  text_layer_enable_screen_text_flow_and_paging(label1, 2);
  text_layer_enable_screen_text_flow_and_paging(loc, 2);
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(64, 64);
}

void deinit() {
  text_layer_destroy(title);
  window_destroy(mainW);
}

int main() {
  init();
  app_event_loop();
  deinit();
  return 0;
}