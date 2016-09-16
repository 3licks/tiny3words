#include <pebble.h>

Window *mainW;
TextLayer *title;
TextLayer *loc;
char *words;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(loc, "Refreshing...");
  DictionaryIterator *out_iter;
  
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if(result == APP_MSG_OK) {
    int value = 0;
    dict_write_int(out_iter, MESSAGE_KEY_refresh, &value, sizeof(int), true);
    result = app_message_outbox_send();
  }
  
}

static void click_config_provider(void *context) {
  // Subcribe to button click events here
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  text_layer_set_text(loc, "Refreshing...");
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
    title = text_layer_create(GRect(0,0,144,168));
    TextLayer *label1 = text_layer_create(GRect(0,50,144,168));
    loc = text_layer_create(GRect(0,75,144,168));
    TextLayer *label2 = text_layer_create(GRect(0,115,144,168));
  #elif defined(PBL_ROUND)
    title = text_layer_create(GRect(0,10,180,180));
    TextLayer *label1 = text_layer_create(GRect(0,50,180,180));
    loc = text_layer_create(GRect(0,72,180,180));
    TextLayer *label2 = text_layer_create(GRect(0,110,180,180));
  #endif
  text_layer_set_text(title, "tiny3words");
  text_layer_set_font(title, fonts_get_system_font(FONT_KEY_GOTHIC_28));
  layer_add_child(window_get_root_layer(mainW), text_layer_get_layer(title));
  text_layer_enable_screen_text_flow_and_paging(title, 6);
  text_layer_set_text(label1, "Your location:");
  text_layer_set_text(label2, "Press select to refresh");
  layer_add_child(window_get_root_layer(mainW), text_layer_get_layer(label1));
  layer_add_child(window_get_root_layer(mainW), text_layer_get_layer(loc));
  layer_add_child(window_get_root_layer(mainW), text_layer_get_layer(label2));
  text_layer_enable_screen_text_flow_and_paging(label1, 6);
  text_layer_enable_screen_text_flow_and_paging(label2, 6);
  text_layer_enable_screen_text_flow_and_paging(loc, 10);
  window_stack_push(mainW, false);
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