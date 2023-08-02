//  Printing/Printing
//
// GtkPrintOperation offers a simple API to support printing
// in a cross-platform way.
//

//  In points  
HEADER_HEIGHT=(10*72/25.4)
HEADER_GAP= (3*72/25.4)

function begin_print (operation, context, data)
  height = context.get_height[] - HEADER_HEIGHT - HEADER_GAP;
  data.lines_per_page = floor (height / data.font_size);
  F=fopen(data.resourcename,mode = 'r');
  lines= F.get_smatrix[];
  F.close[];
  data.lines = lines;
  data.num_lines = size(lines,'*');
  data.num_pages = (data.num_lines - 1) / data.lines_per_page + 1;
  operation.set_n_pages[ data.num_pages];
  operation.user_data = data;
endfunction 

function draw_page (operation, context, page_nr, udata)

  data = operation.user_data;

  // draw the header rectangle 
  cr = context.get_cairo_context[];
  width = context.get_width[];
  cairo_rectangle (cr, 0, 0, width, HEADER_HEIGHT);
  cairo_set_source_rgb (cr, 0.8, 0.8, 0.8);
  cairo_fill_preserve (cr);
  cairo_set_source_rgb (cr, 0, 0, 0);
  cairo_set_line_width (cr, 1);
  cairo_stroke (cr);
  
  // layout = gtk_print_context_create_pango_layout (context);
  layout = context.create_pango_layout[];
  // desc = pango_font_description_from_string ("sans 14");
  desc = pango_font_description_new("sans 14");
  PANGO_SCALE=1024; // XXXXX 
  desc.set_size[ data.font_size * PANGO_SCALE];
  layout.set_font_description[desc];
  
  layout.set_text[data.resourcename]
  psize=  layout.get_pixel_size[];
  text_width=psize(1);text_height=psize(2);
    
  if (text_width > width)
    layout.set_width[width];
    layout.set_ellipsize[PANGO.ELLIPSIZE_START];
    psize= layout.get_pixel_size[]
    text_width=psize(1);text_height=psize(2);
  end
  
  cairo_move_to (cr, (width - text_width) / 2,  (HEADER_HEIGHT - text_height) / 2);
  pango_cairo_show_layout (cr, layout);

  cairo_move_to(cr,100,100);
  pango_cairo_show_layout (cr, layout);
  
  page_str = sprintf ("%d/%d", page_nr + 1, data.num_pages);
  layout.set_text[page_str];
  layout.set_width[ -1];
  psize=  layout.get_pixel_size[];
  text_width=psize(1);text_height=psize(2);
  cairo_move_to (cr, width - text_width - 4, (HEADER_HEIGHT - text_height) / 2);
  pango_cairo_show_layout (cr, layout);

  layout = context.create_pango_layout[];

  desc = pango_font_description_new("monospace");
  PANGO_SCALE=1024; // XXXXX 
  desc.set_size[ data.font_size * PANGO_SCALE];
  layout.set_font_description[desc];
  
  cairo_move_to (cr, 0, HEADER_HEIGHT + HEADER_GAP);
  line = page_nr * data.lines_per_page;
  i= 0;
  
  while %t
    if i < data.lines_per_page && line < data.num_lines then
      layout.set_text[data.lines(line+1)];
      pango_cairo_show_layout (cr, layout);
      cairo_rel_move_to (cr, 0, data.font_size);
      line = line+1;
      i = i+1;
    else
      break;
    end
  end
endfunction 

function end_print (operation, context, user_data)

endfunction 

function demo_printing (do_widget)

  if nargin < 1 then do_widget=gtk_window_new();end
  
  operation = gtk_print_operation_new ();
  data.resourcename = "NSP/demos3/demo_printing.sci";
  data.font_size = 12.0;
  
  operation.connect[ "begin-print", begin_print, data];
  operation.connect[ "draw-page", draw_page, data];
  operation.connect[ "end-print", end_print, data];
  
  operation.set_use_full_page[%f];
  operation.set_unit[GTK.UNIT_POINTS];
  operation.set_embed_page_setup[%t];

  if %f then 
    settings = gtk_print_settings_new ();
    gtk_print_settings_set (settings, GTK.PRINT_SETTINGS_OUTPUT_BASENAME, "gtk-demo");
    operation.set_print_settings[settings];
  end

  ok = operation.run[GTK.PRINT_OPERATION_ACTION_PRINT_DIALOG, do_widget];

  // revoir pour erreur ?
  return;
  
  if ok==2 then 
    dialog = gtk_message_dialog_new (do_widget,
                                     GTK.DIALOG_DESTROY_WITH_PARENT,
                                     GTK.MESSAGE_ERROR,
                                     GTK.BUTTONS_CLOSE,
                                     "%s", error.message);
    g_error_free (error);
    dialog.connect[ "response", gtk_widget_destroy];
    dialog.show[];
  end
endfunction 

