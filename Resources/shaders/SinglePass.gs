#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

in vec4 vPosition[1];

uniform mat4 u_mModel;
uniform mat4 u_mView;
uniform mat4 u_mProj;

vec4 g_objSpaceVtx[8];
vec4 g_ndcSpaceVtx[8];
ivec4 g_faces[6];

void EmitVert(int v)
{
  gl_Position = g_ndcSpaceVtx[v];
  EmitVertex();
}

void EmitFace(int f)
{
  EmitVert(g_faces[f][1]);
  EmitVert(g_faces[f][0]);
  EmitVert(g_faces[f][3]);
  EmitVert(g_faces[f][2]);
  EndPrimitive();
}

void main()
{
  g_faces[0] = ivec4(0,1,3,2);
  g_faces[1] = ivec4(5,4,6,7);
  g_faces[2] = ivec4(4,5,0,1);
  g_faces[3] = ivec4(3,2,7,6);
  g_faces[4] = ivec4(0,3,4,7);
  g_faces[5] = ivec4(2,1,6,5);

  vec4 P = vPosition[0];
  vec4 I = vec4(1,0,0,0);
  vec4 J = vec4(0,1,0,0);
  vec4 K = vec4(0,0,1,0);

  g_objSpaceVtx[0] = P+I+J+K;
  g_objSpaceVtx[1] = P+I-J+K;
  g_objSpaceVtx[2] = P-I-J+K;
  g_objSpaceVtx[3] = P-I+J+K;
  g_objSpaceVtx[4] = P+I+J-K;
  g_objSpaceVtx[5] = P+I-J-K;
  g_objSpaceVtx[6] = P-I-J-K;
  g_objSpaceVtx[7] = P-I+J-K;

  mat4 MVP = u_mProj * u_mView * u_mModel;

  for(int v = 0; v < 8; v++)
    g_ndcSpaceVtx[v] = MVP * g_objSpaceVtx[v];

  for(int f = 0; f < 6; f++)
    EmitFace(f);
}

  