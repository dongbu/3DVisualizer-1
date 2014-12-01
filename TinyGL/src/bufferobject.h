#ifndef BUFFEROBJECT_H
#define BUFFEROBJECT_H

#include <GL/glew.h>

/**
 * @brief The BufferObject class Abstracts the creation and management of
 * buffer objects.
 *
 * This class serves the purpose of abstracting the creation and management of
 * buffer objects of any kind. The user must define the target, size and usage
 * parameters using OpenGL values.
 *
 * The storage for the buffer object is allocated upon construction. However,
 * user may allocate the storage again, should he need it.
 */
class BufferObject
{
public:
  BufferObject(GLenum target, size_t buff_size, GLenum usage);
  ~BufferObject();

  /**
   * @brief allocateStorage Allocates the storage space in the OpenGL device.
   *
   * This method calls the glBufferData function passing the target and usage
   * provided during the construction and the size provided by the buff_size
   * parameter.
   *
   * The buffer must be set as current first by using the bind() method.
   *
   * @param buff_size The size in bytesof the buffer to be allocated.
   */
  void allocateStorage(size_t buff_size);

  /**
   * @brief sendData Uploads the data to the OpenGL device.
   *
   * This method allocates (if not previously allocated) the space in the
   * OpenGL context and uploads the data.
   *
   * The buffer must be set as current first by using the bind() method.
   *
   * @param data A pointer to the data in the memory.
   */
  void sendData(GLvoid* data);

  void bind();
  static void unbind();

  GLuint getId()
  {
    return m_id;
  }

private:
  GLuint m_id;
  GLenum m_target;
  GLenum m_usage;
  size_t m_size;

  bool m_allocated;
};

#endif // BUFFEROBJECT_H
