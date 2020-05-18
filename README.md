# qt-metal
qt metal c++ interface
use apple metal resources in a convenient qt wrapper, 
includes a qwidget promotion full working example implementing voronoi algorithm,

metal programs are coded in resource files:

 metal = new Metal(":/voronoi.metal");
 metal->compile_func("Voronoi");
 metal->compile_func("setPointBox");

VM_Vect uses virtual memory to store interface objects avoiding memory leaks

  // pic, points, colors
  VM_Vect<point> points;
  VM_Vect<color> pic;

metal buffers are integrated in VM_Vect :
 // pic buffer -> w x h
  pic.resize(w * h);
  pic.create_buffer(metal);
  
  // color tiles
  metal->set_func("Voronoi");
  pic.set_buffer(0); // kernel func parameters(pic, points, n_points)
  points.set_buffer(1);
  metal->set_int(n_points, 2);

i've also added a metal.xml sytax highlighting file.
obviously this is only for macos.

uses mtlpp:
/*
 * Copyright 2016-2017 Nikolay Aleksiev. All rights reserved.
 * License: https://github.com/naleksiev/mtlpp/blob/master/LICENSE
 */
