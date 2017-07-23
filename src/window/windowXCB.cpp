#include "windowXCB.h"
#include "RealEngine/keyboard.h"

#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#include <xcb/xcb_image.h>
#include <xcb/xcb_keysyms.h>
#include <xcb/xcb_atom.h>
#include <xcb/xcb_icccm.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

namespace re{

Key keysym_to_key(xcb_keysym_t keysym);



static xcb_format_t *
find_format (xcb_connection_t * c, uint8_t depth, uint8_t bpp)
{
 const xcb_setup_t *setup = xcb_get_setup(c);
 xcb_format_t *fmt = xcb_setup_pixmap_formats(setup);
 xcb_format_t *fmtend = fmt + xcb_setup_pixmap_formats_length(setup);
 for(; fmt != fmtend; ++fmt)
  if((fmt->depth == depth) && (fmt->bits_per_pixel == bpp)) {
   /* printf("fmt %p has pad %d depth %d, bpp %d\n",
      fmt,fmt->scanline_pad, depth,bpp); */
   return fmt;
  }
 return 0;
}

// TODO rewrite this function!!!
void
fillimage(unsigned char *p, int width, int height)
{
 int i, j;
 srand(0);
 for(i=0; i < width; i++)
 {
  for(j=0; j < height; j++)
  {
   if((i < 256)&&(j < 256))
   {
    *p++=rand()%256; // blue
    *p++=rand()%256; // green
    *p++=rand()%256; // red
   } else {
    *p++=i%256; // blue
    *p++=j%256; // green
    if(i < 256)
     *p++=i%256; // red
    else if(j < 256)
     *p++=j%256; // red
    else
     *p++=(256-j)%256; // red
   }
   p++; /* unused byte */
  }
 }
}

xcb_image_t *CreateTrueColorImage(xcb_connection_t *c, int width, int height){
 const xcb_setup_t *setup = xcb_get_setup(c);
 unsigned char *image32=(unsigned char *)malloc(width*height*4);
 xcb_format_t *fmt = find_format(c, 24, 32);
 if (fmt == NULL)
  return NULL;

 fillimage(image32, width, height);


 return xcb_image_create(width,
    height,
    XCB_IMAGE_FORMAT_Z_PIXMAP,
    fmt->scanline_pad,
    fmt->depth,
    fmt->bits_per_pixel,
    0,
    (xcb_image_order_t)setup->image_byte_order,
    XCB_IMAGE_ORDER_LSB_FIRST,
    image32,
    width*height*4,
    image32);
}

WindowXCB::WindowXCB(uint w, uint h)
    // : event_handler(nullptr)
{
    width = w;
    height = h;
    fps_count_max = 60;
    set_fps(fps_count_max);

    conn = xcb_connect (NULL, NULL);

    /* Get the first screen */
    screen = xcb_setup_roots_iterator (xcb_get_setup (conn)).data;

    /* Ask for our window's Id */
    window = screen->root;

    color_context = xcb_generate_id (conn);
    mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
    values[0] = screen->black_pixel;
    values[1] = 0;
    xcb_create_gc (conn, color_context, window, mask, values);

    window = xcb_generate_id(conn);

    /* Create the window */
    mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_EXPOSURE       | 
                XCB_EVENT_MASK_BUTTON_PRESS   |
                XCB_EVENT_MASK_BUTTON_RELEASE | 
                XCB_EVENT_MASK_ENTER_WINDOW   | 
                XCB_EVENT_MASK_LEAVE_WINDOW   |
                XCB_EVENT_MASK_KEY_PRESS      | 
                XCB_EVENT_MASK_KEY_RELEASE    |
                XCB_EVENT_MASK_POINTER_MOTION; 
    xcb_create_window (conn, 
        XCB_COPY_FROM_PARENT, 
        window,                  
        screen->root,         
        0, 0,                 
        width, height,             
        10,                   
        XCB_WINDOW_CLASS_INPUT_OUTPUT, 
        screen->root_visual,  
        mask, values);             

    /* Map the window on the screen */
    xcb_map_window (conn, window);

    xcb_flush (conn);

    // keyboard
    uint16_t m1, m2;
    if (xkb_x11_setup_xkb_extension(conn, XKB_X11_MIN_MAJOR_XKB_VERSION, XKB_X11_MIN_MINOR_XKB_VERSION,
                                    XKB_X11_SETUP_XKB_EXTENSION_NO_FLAGS, 
                                    &m1, &m2, NULL, NULL) == 0){
        std::cerr << "error in setup xkb extension\n" << m1 << " " << m2 << "\n";
        exit(1);
    }

    ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!ctx){
        std::cerr << "cannot make xkb context. Keyboard event cannot be handled. Abort\n";
        exit(1);
    }
    int32_t device_id;
    device_id = xkb_x11_get_core_keyboard_device_id(conn);
    if (device_id == -1){
        std::cerr << "device_id is -1";
        exit(1);
    }
    keymap = xkb_x11_keymap_new_from_device(ctx, conn, device_id,
                                            XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!keymap){
        std::cerr << "cannot make xkb keymap. Keyboard event cannot be handled. Abort\n";
        exit(1);
    }
    state = xkb_x11_state_new_from_device(keymap, conn, device_id);
    if (!state){
        std::cerr << "cannot make xkb state. Keyboard event cannot be handled. Abort\n";
        exit(1);
    }

    // image tmp code
    back_image = CreateTrueColorImage(conn, width, height);
    if (back_image == NULL){
        std::cerr << "image create failed\n";
    }
    image32 = back_image->data;
    // hints = xcb_alloc_size_hints();
    // xcb_size_hints_set_max_size(hints, image->width, image->height);
    // xcb_size_hints_set_min_size(hints, image->width, image->height);
    // xcb_set_wm_size_hints(conn, window, WM_NORMAL_HINTS, hints);
    pmap = xcb_generate_id(conn);
    xcb_create_pixmap(conn, 24, pmap, window, back_image->width, back_image->height);
    gc = xcb_generate_id (conn);
    mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    values[0] = screen->black_pixel;
    values[1] = 0xffffff;
    xcb_create_gc (conn, gc, pmap, mask, values);
    // xcb_image_put(conn, pmap, gc, back_image, 0, 0, 0);

    color_context = xcb_generate_id (conn);
    mask = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND;
    values[0] = screen->white_pixel;
    values[1] = 0xffffff;
    xcb_create_gc (conn, color_context, window, mask, values);

    Color color = re::WHITE;
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            xcb_image_put_pixel(back_image, i, j, color.to_int());
        }
    }
    xcb_image_put(conn, pmap, gc, back_image, 0, 0, 0);

    colormap = screen->default_colormap;

    xcb_map_window (conn, window);
    xcb_flush (conn);
}

WindowXCB::~WindowXCB(){
    xkb_state_unref(state);
    xkb_keymap_unref(keymap);
    xkb_context_unref(ctx);
    xcb_free_pixmap(conn, pmap);
}

void WindowXCB::run_events(){
    xcb_generic_event_t *e;

    while ((e = xcb_poll_for_event(conn))) {
        switch (e->response_type & ~0x80) {
        case XCB_BUTTON_PRESS: {
            xcb_button_press_event_t *ev = (xcb_button_press_event_t *)e;
            if (event_handler)
                event_handler->on_button_pressed((int)ev->detail);
            break;
        }
        case XCB_KEY_PRESS: {
            xcb_key_press_event_t *ev = (xcb_key_press_event_t *)e;
            xkb_keysym_t keysym = xkb_state_key_get_one_sym(state, ev->detail);
            if (event_handler)
                event_handler->on_key_pressed(keysym_to_key(keysym));
            break;
        }
        case XCB_KEY_RELEASE: {
            xcb_key_press_event_t *ev = (xcb_key_press_event_t *)e;
            xkb_keysym_t keysym = xkb_state_key_get_one_sym(state, ev->detail);
            if (event_handler)
                event_handler->on_key_released(keysym_to_key(keysym));
            break;
        }
        case XCB_EXPOSE:{
            xcb_expose_event_t *ev = (xcb_expose_event_t *)e;
            // printf ("expose %d,%d - %d,%d\n",
                // ev->x,ev->y,ev->width,ev->height);
            // xcb_copy_area(conn, pmap, window, gc,
            //         ev->x,
            //         ev->y,
            //         ev->x,
            //         ev->y,
            //         ev->width,
            //         ev->height);
            // xcb_flush (conn);
            // image32+=16;
            break;
        }
        case XCB_MOTION_NOTIFY: {
            xcb_motion_notify_event_t *ev = (xcb_motion_notify_event_t *)e;
            if (event_handler)
                event_handler->on_mouse_move(ev->event_x, ev->event_y);
            break;
        }
        default: {
            /* Unknown event type, ignore it */
            break;
        }
        }
        /* Free the Generic Event */
        free (e);
    }
}

void WindowXCB::display(){
    xcb_image_put(conn, pmap, gc, back_image, 0, 0, 0);
    xcb_copy_area(conn, pmap, window, gc, 0, 0, 0, 0, width, height);
    xcb_flush (conn);

    usleep(40000);
}

void WindowXCB::background(Color color){
    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            xcb_image_put_pixel(back_image, i, j, color.to_int());
        }
    }
    xcb_image_put(conn, pmap, gc, back_image, 0, 0, 0);
    xcb_flush(conn);
}

void WindowXCB::draw_rectangle(int x0, int y0, int w, int h, Color color){
    for (int i = y0; i < y0 + h; i++){
        for (int j = x0; j < x0 + w; j++){
            if (i < 0 || i >= height || j < 0 || j >= width){
                continue;
            }
            xcb_image_put_pixel(back_image, j, i, color.to_int());
        }
    }
    xcb_image_put(conn, pmap, gc, back_image, 0, 0, 0);
    xcb_flush(conn);
}

void WindowXCB::draw_circle(int x0, int y0, int r, Color color){
    for (int i = y0 - r; i < y0 + r; i++){
        for (int j = x0 - r; j < x0 + r; j++){
            if (i < 0 || i >= height || j < 0 || j >= width){ 
                continue;
            }
            // TODO make it circle...
            if ((i - y0)*(i - y0) + (j - x0)*(j - x0) > r*r){
                continue;
            }
            xcb_image_put_pixel(back_image, j, i, color.to_int());
        }
    }
    xcb_image_put(conn, pmap, gc, back_image, 0, 0, 0);
    xcb_flush(conn);
}

void WindowXCB::draw_line(int x0, int y0, int x1, int y1, Color color) {

}


void WindowXCB::draw_image(int x0, int y0, ImagePtr im){
    for (int x = x0; x < x0 + im->w; x++){
        for (int y = y0; y < y0 + im->h; y++){
            xcb_image_put_pixel(back_image, x, y, im->get_pix_color(x-x0, y-y0).to_int());
        }
    }
}


void WindowXCB::register_event_handler(IBaseAppPtr e){
    if (event_handler){
        std::cerr << "replace event handler\n";
    }
    event_handler = e;
}

void WindowXCB::set_fps(uint fps_count){

}

uint WindowXCB::get_fps(){
    return fps;
}

uint WindowXCB::get_width(){
    return width;
}

uint WindowXCB::get_height(){
    return height;
}


Key keysym_to_key(xcb_keysym_t keysym){
    switch (keysym){
        case XK_Shift_L:        return Key::LShift; 
        case XK_Shift_R:        return Key::RShift; 
        case XK_Control_L:      return Key::LControl; 
        case XK_Control_R:      return Key::RControl; 
        case XK_Alt_L:          return Key::LAlt; 
        case XK_Alt_R:          return Key::RAlt; 
        case XK_Super_L:        return Key::LSystem; 
        case XK_Super_R:        return Key::RSystem; 
        case XK_Menu:           return Key::Menu; 
        case XK_Escape:         return Key::Escape; 
        case XK_semicolon:      return Key::SemiColon; 
        case XK_slash:          return Key::Slash; 
        case XK_equal:          return Key::Equal; 
        case XK_minus:          return Key::Dash; 
        case XK_bracketleft:    return Key::LBracket; 
        case XK_bracketright:   return Key::RBracket; 
        case XK_comma:          return Key::Comma; 
        case XK_period:         return Key::Period; 
        case XK_apostrophe:     return Key::Quote; 
        case XK_backslash:      return Key::BackSlash; 
        case XK_grave:          return Key::Tilde; 
        case XK_space:          return Key::Space; 
        case XK_Return:         return Key::Return; 
        case XK_BackSpace:      return Key::BackSpace; 
        case XK_Tab:            return Key::Tab; 
        case XK_Prior:          return Key::PageUp; 
        case XK_Next:           return Key::PageDown; 
        case XK_End:            return Key::End; 
        case XK_Home:           return Key::Home; 
        case XK_Insert:         return Key::Insert; 
        case XK_Delete:         return Key::Delete; 
        case XK_KP_Add:         return Key::Add; 
        case XK_KP_Subtract:    return Key::Subtract; 
        case XK_KP_Multiply:    return Key::Multiply; 
        case XK_KP_Divide:      return Key::Divide; 
        case XK_Pause:          return Key::Pause; 
        case XK_F1:             return Key::F1; 
        case XK_F2:             return Key::F2; 
        case XK_F3:             return Key::F3; 
        case XK_F4:             return Key::F4; 
        case XK_F5:             return Key::F5; 
        case XK_F6:             return Key::F6; 
        case XK_F7:             return Key::F7; 
        case XK_F8:             return Key::F8; 
        case XK_F9:             return Key::F9; 
        case XK_F10:            return Key::F10; 
        case XK_F11:            return Key::F11; 
        case XK_F12:            return Key::F12; 
        case XK_F13:            return Key::F13; 
        case XK_F14:            return Key::F14; 
        case XK_F15:            return Key::F15; 
        case XK_Left:           return Key::Left; 
        case XK_Right:          return Key::Right; 
        case XK_Up:             return Key::Up; 
        case XK_Down:           return Key::Down; 
        case XK_KP_Insert:      return Key::Numpad0; 
        case XK_KP_End:         return Key::Numpad1; 
        case XK_KP_Down:        return Key::Numpad2; 
        case XK_KP_Page_Down:   return Key::Numpad3; 
        case XK_KP_Left:        return Key::Numpad4; 
        case XK_KP_Begin:       return Key::Numpad5; 
        case XK_KP_Right:       return Key::Numpad6; 
        case XK_KP_Home:        return Key::Numpad7; 
        case XK_KP_Up:          return Key::Numpad8; 
        case XK_KP_Page_Up:     return Key::Numpad9; 
        case XK_a:              return Key::A; 
        case XK_b:              return Key::B; 
        case XK_c:              return Key::C; 
        case XK_d:              return Key::D; 
        case XK_e:              return Key::E; 
        case XK_f:              return Key::F; 
        case XK_g:              return Key::G; 
        case XK_h:              return Key::H; 
        case XK_i:              return Key::I; 
        case XK_j:              return Key::J; 
        case XK_k:              return Key::K; 
        case XK_l:              return Key::L; 
        case XK_m:              return Key::M; 
        case XK_n:              return Key::N; 
        case XK_o:              return Key::O; 
        case XK_p:              return Key::P; 
        case XK_q:              return Key::Q; 
        case XK_r:              return Key::R; 
        case XK_s:              return Key::S; 
        case XK_t:              return Key::T; 
        case XK_u:              return Key::U; 
        case XK_v:              return Key::V; 
        case XK_w:              return Key::W; 
        case XK_x:              return Key::X; 
        case XK_y:              return Key::Y; 
        case XK_z:              return Key::Z; 
        case XK_0:              return Key::Num0; 
        case XK_1:              return Key::Num1; 
        case XK_2:              return Key::Num2; 
        case XK_3:              return Key::Num3; 
        case XK_4:              return Key::Num4; 
        case XK_5:              return Key::Num5; 
        case XK_6:              return Key::Num6; 
        case XK_7:              return Key::Num7; 
        case XK_8:              return Key::Num8; 
        case XK_9:              return Key::Num9; 
        default:                return Key::Unknown; 
    }
}

} // namespace re