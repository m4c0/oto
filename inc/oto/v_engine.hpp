#pragma once

namespace oto {
  struct v_engine {
    virtual ~v_engine() = default;
    virtual void run_frame() = 0;
  };

  v_engine * create_engine();
}
