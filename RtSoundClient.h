#pragma once
#include "RtSoundData.h"
#include "RtSoundSetup.h"
#include <cassert>
#include <memory>

class RtSoundClient {
  friend class RtSoundProvider;

public:
  RtSoundClient(int priority = 0) : _priority(priority) {}
  virtual ~RtSoundClient() = default;

  inline void setPriority(int priority) { _priority = priority; }
  inline int priority() const { return _priority; };

  // Stream Setup
  // ---------------------------------------------------------------------------
  inline void setStreamSetup(std::weak_ptr<RtSoundSetup> setup) {
    assert(setup.lock() != nullptr);
    _streamSetup.swap(setup);
  }

  const RtSoundSetup &streamSetup() const {
    const auto ptr{_streamSetup.lock()};
    assert(ptr != nullptr);
    return (*ptr);
  }

  // Stream Data
  // ---------------------------------------------------------------------------
  inline void setStreamData(std::weak_ptr<RtSoundData> data) {
    assert(data.lock() != nullptr);
    _streamData.swap(data);
  }

  const RtSoundData &streamData() const {
    const auto ptr{_streamData.lock()};
    assert(ptr != nullptr);
    return (*ptr);
  }

  inline bool hasStreamData() const { return _streamData.lock() != nullptr; }

protected:
  virtual void updateSoundDevices(const std::vector<RtAudio::DeviceInfo> &) {}
  virtual void configureStream(RtSoundSetup &) {}
  virtual void applyStreamConfig(const RtSoundSetup &) {}
  virtual void streamDataReady(const RtSoundData &) {}

private:
  int _priority{};
  std::weak_ptr<RtSoundSetup> _streamSetup{};
  std::weak_ptr<RtSoundData> _streamData{};
};
