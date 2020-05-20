# qt-metal
qt metal c++ interface

use apple metal resources in a convenient qt wrapper, 

virtual memory manager is implemented in a VM_Vect vector alike class,

 // pic, points, colors
  VM_Vect<point> points;
  VM_Vect<color> pic;

compiled default.metallib file should be copied to the same executable folder

includes a qwidget promotion full working example implementing voronoi algorithm,

 // pic buffer -> w x h
  pic.resize(w * h);

  // color tiles
  metal->compileFunc("Voronoi");

  pic.create_copy_buffer(metal);
  points.create_copy_buffer(metal);

  pic.set_buffer(0);
  points.set_buffer(1);
  metal->set_int(points.size(), 2);

  metal->runThreadsWidth(w, h);

  // points
  metal->compileFunc("setPointBox");

  pic.create_copy_buffer(metal);
  points.create_copy_buffer(metal);

  pic.set_buffer(0);
  points.set_buffer(1);
  metal->set_int(w, 2);

  metal->runThreadsWidth(points.size(), 1);


i've also added a metal.xml sytax highlighting file.
obviously this is only for macos.

inspired in Nikolay's mtlpp:  https://github.com/naleksiev/mtlpp
