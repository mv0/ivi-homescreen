// Copyright 2020 Toyota Connected North America
// Copyright 2022 Konsulko Group
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "agl_shell.h"

#include <flutter/fml/logging.h>
#include <flutter/standard_method_codec.h>

#include "engine.h"
#include "wayland/display.h"

#include <iostream>

void AglShell::OnPlatformMessage(const FlutterPlatformMessage* message,
                                    void* userdata) {
  std::unique_ptr<std::vector<uint8_t>> result;
  auto engine = reinterpret_cast<Engine*>(userdata);
  auto& codec = flutter::StandardMethodCodec::GetInstance();
  auto obj = codec.DecodeMethodCall(message->message, message->message_size);

  auto method = obj->method_name();
  if (method == kMethodActivateApp) {
    if (obj->arguments()->IsNull()) {
      result = codec.EncodeErrorEnvelope("argument_error", "Invalid Arguments");
      goto done;
    }

    auto args = std::get_if<flutter::EncodableMap>(obj->arguments());
    if (!args) {
      result = codec.EncodeErrorEnvelope("argument_error", "Invalid Arguments");
      goto done;
    }

    std::string app_id;
    auto it = args->find(flutter::EncodableValue("app_id"));
    if (it != args->end()) {
      app_id = std::get<std::string>(it->second);
    }

    int32_t index = 0;
    it = args->find(flutter::EncodableValue("index"));
    if (it != args->end()) {
      index = std::get<int32_t>(it->second);
    }

    if (app_id.empty() || index < 0) {
      result = codec.EncodeErrorEnvelope("argument_error", "Invalid Arguments");
      goto done;
    }

    auto display = engine->GetDisplay();
    if (display) {
      display->AglShellDoActivate(app_id, index);
    }
    auto val = flutter::EncodableValue(true);
    result = codec.EncodeSuccessEnvelope(&val);
  } else {
    FML_DLOG(INFO) << "AglShell: " << method << " is unhandled";
    result = codec.EncodeErrorEnvelope("unhandled_method", "Unhandled Method");
  }

 done:
  engine->SendPlatformMessageResponse(message->response_handle, result->data(),
                                      result->size());
}
