// Resources
//
//

function window = demo_resources(do_widget)
// Create an image from images stored in file
  
  function demo_add_image(image,ok,title,vbox)
    label = gtk_label_new();
    label.set_markup["<u>"+title+"</u>"];
    vbox.pack_start[ label,expand=%f,fill=%f,padding=0]
    if ok then
      frame = gtk_frame_new();
      frame.set_shadow_type[GTK.SHADOW_IN];
      vbox.pack_start[frame,expand=%f,fill=%f,padding=0]
      frame.add[image]
    else
      label = gtk_label_new();
      label.set_markup["<u>"+'failed to create image'+"</u>"];
      vbox.pack_start[ label,expand=%f,fill=%f,padding=0]
    end
  endfunction

  window = gtk_window_new()
  window.set_title[" images "];
  window.set_border_width[  8]
  vbox = gtk_box_new("vertical",spacing=8);
  vbox.set_border_width[  8]
  window.add[  vbox]
  
  // image = gtk_image_new_from_resource("/pixbufs/gnome-foot.png");
  image = gtk_image_new_from_resource("/pixbufs/tumbi48.png");
  demo_add_image(image,%t,"gtk_image_new_from_resource",vbox)
    
  window.show_all[]
endfunction

