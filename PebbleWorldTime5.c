#include "pebble.h" 

#define NUM_MENU_SECTIONS 2
#define NUM_MENU_ICONS 3
#define NUM_FIRST_MENU_ITEMS 3
#define NUM_SECOND_MENU_ITEMS 1
#define NUM_LOCATIONS 4
  
char * cities[NUM_LOCATIONS] = {"Local", "Abu Dhabi","Barcelona", "Cairo"};
TextLayer *locationname;
TextLayer *timetext_world;
Window *timewindow;
TextLayer *timetext_layer;
static char * astring = "                           ";
time_t timeabu;
static char * tstring = "                           ";
time_t timevalue;
int timeoffset[NUM_LOCATIONS] = {0,39600,32400};
int location = 0;

void updatetime(void) {

	
	time(&timevalue);
  timevalue = timevalue + timeoffset[location];
  strftime	( tstring, 6, "%I:%M", localtime(&timevalue));
  // Set the text, font, and text alignment
	text_layer_set_text(timetext_layer, tstring);
  text_layer_set_text(locationname, cities[location]);
	  
} 

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  location = location - 1;
  if (location < 0)
        location = 0;
  updatetime();
  
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  location = location + 1;
  if (location >= NUM_LOCATIONS)
        location = NUM_LOCATIONS - 1;
  updatetime();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}
 
void time_window_load(Window* timewindow) {
  
  timetext_layer = text_layer_create(GRect(0, 28, 144, 154));
  locationname= text_layer_create(GRect (0, 0, 144, 28));
	
	time(&timevalue);
  timevalue = timevalue + timeoffset[location];
  strftime	( tstring, 6, "%I:%M", localtime(&timevalue));
  // Set the text, font, and text alignment
	text_layer_set_text(timetext_layer, tstring);
	text_layer_set_font(timetext_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(timetext_layer, GTextAlignmentCenter);
  	
  text_layer_set_text(locationname, cities[location]);
	text_layer_set_font(locationname, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(locationname, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(timewindow), text_layer_get_layer(timetext_layer));
  
  layer_add_child(window_get_root_layer(timewindow), text_layer_get_layer(locationname));
}


void time_window_unload(Window *timewindow) {
  text_layer_destroy(timetext_layer);
  text_layer_destroy(locationname);
}



void displaytime(void) {
	// Create a window and text layer
  
  
	timewindow = window_create();
	
  window_set_window_handlers(timewindow, (WindowHandlers) {
    .load = time_window_load,
    .unload = time_window_unload,
  });
  
  window_set_click_config_provider(timewindow, click_config_provider);
	
  // Push the window
	window_stack_push(timewindow, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
} 



/*void world(void) {
  
  time(&timeabu);
  timeabu = timeabu + timeoffset[0];
  strftime	( astring, 6, "%I:%M", localtime(&timeabu));
	timewindow = window_create();
	timetext_world = text_layer_create(GRect(0, 0, 144, 154));
	
	// Set the text, font, and text alignment
	text_layer_set_text(timetext_world, astring);
	text_layer_set_font(timetext_world, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(timetext_world, GTextAlignmentCenter);
	
	// Add the text layer to the window
	layer_add_child(window_get_root_layer(timewindow), text_layer_get_layer(timetext_world));

  window_set_click_config_provider(timewindow, click_config_provider);
	
  // Push the window
	window_stack_push(timewindow, true);
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");

	
} */








static Window *window;

// This is a menu layer
// You have more control than with a simple menu layer
static MenuLayer *menu_layer;

// Menu items can optionally have an icon drawn with them
static GBitmap *globe_icon;

//static int current_icon = 0;

// You can draw arbitrary things in a menu item such as a background
static GBitmap *menu_background;

// A callback is used to specify the amount of sections of menu items
// With this, you can dynamically add and remove sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

// Each section has a number of items;  we use a callback to specify this
// You can also dynamically add and remove items using this
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  switch (section_index) {
    case 0:
      return NUM_FIRST_MENU_ITEMS; break;

    case 1:
      return NUM_SECOND_MENU_ITEMS; break;

    default:
      return 0;
  }
}

// A callback is used to specify the height of the section header
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  // This is a define provided in pebble.h that you may use for the default height
//  return MENU_CELL_BASIC_HEADER_HEIGHT;
  return 14;
}

// Here we draw what each header is
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  // Determine which section we're working with
  switch (section_index) {
    case 0:
      // Draw title text in the section header
      menu_cell_basic_header_draw(ctx, cell_layer, "World Time");
      break;
  }
}

// This is the menu item draw callback where you specify what each item should look like
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in
  switch (cell_index->section) {
    case 0:
      // Use the row to specify which item we'll draw
      switch (cell_index->row) {
        case 0:
          // This is a basic menu item with a title and subtitle
          menu_cell_basic_draw(ctx, cell_layer, "Local Time", "Select For Local Time", NULL);
          break;

        case 1:
          // This is a basic menu icon with a cycling icon
          menu_cell_basic_draw(ctx, cell_layer, "World Time", "Select For World Time", globe_icon);
          break;

        case 2:
          // Here we use the graphics context to draw something different
          // In this case, we show a strip of a watchface's background
 //         graphics_draw_bitmap_in_rect(ctx, menu_background,
 //             (GRect){ .origin = GPointZero, .size = layer_get_frame((Layer*) cell_layer).size });
          break;
      }
      break;

    case 1:
      switch (cell_index->row) {
        case 0:
          // There is title draw for something more simple than a basic menu item
          menu_cell_title_draw(ctx, cell_layer, "Final Item");
          break;
      }
  }
}

// Here we capture when a user selects a menu item
void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {

  location = cell_index->row;
  displaytime();
  
}

// This initializes the menu upon window load
void window_load(Window *window) {
  // Now we prepare to initialize the menu layer
  // We need the bounds to specify the menu layer's viewport size
  // In this case, it'll be the same as the window's
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
  globe_icon = gbitmap_create_with_resource(RESOURCE_ID_GLOBE_ICON);
  
  // Create the menu layer
  menu_layer = menu_layer_create(bounds);

  // Set all the callbacks for the menu layer
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  // Add it to the window for display
  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

void window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(menu_layer);

  gbitmap_destroy(globe_icon);

  // And cleanup the background
//  gbitmap_destroy(menu_background);
}

int main(void) {
  window = window_create();

  // Setup the window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  window_stack_push(window, true /* Animated */);

  app_event_loop();

  window_destroy(window);
}
