//
//  Arcball.h
//  Arcball
//
//  Created by Saburo Okita on 12/03/14.
//  Copyright (c) 2014 Saburo Okita. All rights reserved.
//

#ifndef __Arcball__Arcball__
#define __Arcball__Arcball__

#include <iostream>

#include <GLFW/glfw3.h>
#include <iup/iup.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
/*#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_inverse.hpp>*/

class Arcball {

public:
  Arcball( int window_width, int window_height, GLfloat roll_speed = 1.0f, bool x_axis = true, bool y_axis = true );
  glm::vec3 toScreenCoord( double x, double y );

  void mouseButtonCallback(Ihandle* window, int button, int pressed);
  void cursorCallback(Ihandle* window, double x, double y );

  glm::mat4 createViewRotationMatrix();
  glm::mat4 createModelRotationMatrix( glm::mat4& view_matrix );

private:
  int windowWidth;
  int windowHeight;
  int mouseEvent;
  GLfloat rollSpeed;
  GLfloat angle ;
  glm::vec3 prevPos;
  glm::vec3 currPos;
  glm::vec3 camAxis;

  bool xAxis;
  bool yAxis;
};

#endif /* defined(__Arcball__Arcball__) */
