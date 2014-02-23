/*
 * pebble pedometer+
 * @author jathusant
 */

#include <run.h>
	
Window *window;
Window *menu_window;

SimpleMenuLayer *pedometer_settings;
SimpleMenuItem menu_items[5];
SimpleMenuSection menu_sections[1];
char *item_names[5] = {"Start", "Step Goal", "Pedometer Theme", "About", "Developed By"};
char *item_sub[5] = {"Lets Go!", "Not Set", "Current: ", "v1.0-RELEASE", "Jathusan T."};

TextLayer *main_message;
TextLayer *main_message2;
TextLayer *sub_message;
TextLayer *sub_message2;

//used for themes
bool isDark = false;
bool stepLimitExists = false;
char *theme = "Light";
int stepGoal = 1000;
int pedometerCount = 0;


////////////////////////////////////////////////////////////////////
// Settings Menu
///////////////////////////////////////////////////////////////////

void m_up_click_handler(ClickRecognizerRef recognizer, void *context){
}
 
void m_down_click_handler(ClickRecognizerRef recognizer, void *context){
}
 
void m_select_click_handler(ClickRecognizerRef recognizer, void *context){
}

void menu_click_config_provider(void *context){
    window_single_click_subscribe(BUTTON_ID_UP, m_up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, m_down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, m_select_click_handler);
}

void setup_menu_items(){
	for (int i = 0; i < (int)(sizeof(item_names) / sizeof(item_names[0])); i++){
		menu_items[i] = (SimpleMenuItem){
    		.title = item_names[i],
			.subtitle = item_sub[i],
		};
		if (i==1 && stepGoal>0){
			static char buf[]="123456";
			snprintf(buf, sizeof(buf), "%d", stepGoal); 
			menu_items[i].subtitle = buf;
		} else if (i==2){
			char* new_string;
			new_string = malloc(strlen(item_sub[i])+strlen(theme)+1);
			strcpy(new_string, item_sub[i]);
			strcat(new_string, theme);
			menu_items[i].subtitle = new_string;
		}
    }
}

void setup_menu_sections(){
	menu_sections[0] =  (SimpleMenuSection){
		.items = menu_items,
		.num_items = sizeof(menu_items) / sizeof(menu_items[0])
	};
}

void setup_menu_window(){
	menu_window = window_create();
	
	window_set_window_handlers(menu_window, (WindowHandlers) {
      .load = settings_load,
      .unload = settings_unload,
    });
	
	window_set_click_config_provider(menu_window, menu_click_config_provider);
}

void settings_load(Window *window){	
	if (isDark){
    	//window_set_background_color(window, GColorBlack);
	}
	
	Layer *layer = window_get_root_layer(menu_window);
	
	pedometer_settings = simple_menu_layer_create(layer_get_bounds(layer), menu_window, menu_sections, 1, NULL);
	layer_add_child(layer,simple_menu_layer_get_layer(pedometer_settings));
}

void settings_unload(Window *window){
    //empty for now
}

////////////////////////////////////////////////////////////////////
// Splash Menu
///////////////////////////////////////////////////////////////////
void handleColorSelect(){
    if (isDark){
        isDark = false;
		theme = "Light";
        window_set_background_color(window, GColorWhite);
        text_layer_set_text_color(main_message, GColorBlack);
        text_layer_set_text_color(main_message2, GColorBlack);
        text_layer_set_text_color(sub_message, GColorBlack);
        text_layer_set_text_color(sub_message2, GColorBlack);
    } else {
        isDark = true;
		theme = "Dark";
        window_set_background_color(window, GColorBlack);
        text_layer_set_text_color(main_message, GColorWhite);
        text_layer_set_text_color(main_message2, GColorWhite);
        text_layer_set_text_color(sub_message, GColorWhite);
        text_layer_set_text_color(sub_message2, GColorWhite);
    }   
}

void up_click_handler(ClickRecognizerRef recognizer, void *context){

}
 
void down_click_handler(ClickRecognizerRef recognizer, void *context){

}
 
void select_click_handler(ClickRecognizerRef recognizer, void *context){
	window_stack_pop(true);
	setup_menu_items();
	setup_menu_sections();
	setup_menu_window();
	window_stack_push(menu_window, true);
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

void window_load(Window *window){
    window_set_background_color(window, GColorBlack);
    
    //initializing text layers
    main_message = text_layer_create(GRect(0, 0, 150, 150));
    main_message2 = text_layer_create(GRect(0, 30, 150, 150));
    sub_message = text_layer_create(GRect(0,60,150,150));
    sub_message2 = text_layer_create(GRect(0,110,150,150));
    
    //"Welcome"
    text_layer_set_background_color(main_message, GColorClear);
    text_layer_set_text_color(main_message, GColorWhite);
    text_layer_set_font(main_message, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_LT_30)));
    layer_add_child(window_get_root_layer(window), (Layer*) main_message);
    
    //"to Cadenza"
    text_layer_set_background_color(main_message2, GColorClear);
    text_layer_set_text_color(main_message2, GColorWhite);
    text_layer_set_font(main_message2, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_LT_15)));
    layer_add_child(window_get_root_layer(window), (Layer*) main_message2);
    
    //"select a theme"
    text_layer_set_background_color(sub_message, GColorClear);
    text_layer_set_text_color(sub_message, GColorWhite);
    text_layer_set_font(sub_message, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_LT_30)));
    layer_add_child(window_get_root_layer(window), (Layer*) sub_message);
    
    //"current theme:"
    text_layer_set_background_color(sub_message2, GColorClear);
    text_layer_set_text_color(sub_message2, GColorWhite);
    text_layer_set_font(sub_message2, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_ROBOTO_LT_15)));
    layer_add_child(window_get_root_layer(window), (Layer*) sub_message2);
    
    //setting text
    text_layer_set_text(main_message, " Welcome");
    text_layer_set_text(main_message2, "      to Pedometer+");
    text_layer_set_text(sub_message, "    <logo>");
    text_layer_set_text(sub_message2, "         SELECT to\n          continue!");
} 

void window_unload(Window *window){
    text_layer_destroy(main_message);
    text_layer_destroy(main_message);
    text_layer_destroy(main_message2);
    text_layer_destroy(sub_message);
    text_layer_destroy(sub_message2);
}

//Initializer/////////////////////////////////////////////////////////////////

void handle_init(void) {
    window = window_create();
	
    window_set_window_handlers(window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
	
    window_set_click_config_provider(window, click_config_provider);
	window_set_fullscreen(window, true);
    window_stack_push(window, true);
}

void handle_deinit(void) {
    window_destroy(window);
}