/*
 * Copyright 2020 Toyota Connected North America
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <memory>
#include <vector>

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <flutter_embedder.h>
#include "constants.h"

class Engine;

class Texture {
 public:
  Texture(uint32_t id,
          uint32_t target,
          uint32_t format,
          VoidCallback create_callback,
          VoidCallback dispose_callback,
          int width = 0,
          int height = 0);

  ~Texture();

  Texture(const Texture&) = delete;

  const Texture& operator=(const Texture&) = delete;

  void SetEngine(const std::shared_ptr<Engine>& engine);

  void GetFlutterOpenGLTexture(FlutterOpenGLTexture* texture_out,
                               int width,
                               int height);

  int64_t Create(int width, int height);
  void Dispose();
  void Enable(uint32_t name);
  void Disable();
  void FrameReady();
  [[maybe_unused]] [[nodiscard]] int64_t GetTextureId() const { return m_name; }

 protected:
  std::shared_ptr<Engine> m_flutter_engine;
  [[maybe_unused]] bool m_enabled;
  int64_t m_id;
  int64_t m_name;
  uint32_t m_target;
  uint32_t m_format;
  [[maybe_unused]] int m_width;
  [[maybe_unused]] int m_height;

  [[maybe_unused]] EGLSurface m_surface{};

  bool m_draw_next;

 private:
  const VoidCallback m_create_callback;
  const VoidCallback m_dispose_callback;
};