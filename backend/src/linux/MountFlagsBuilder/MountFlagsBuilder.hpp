#pragma once

class MountMode;

class MountFlagsBuilder {
    int flags;

    static constexpr int ZeroFlags() noexcept;
    static constexpr int ReadOnly() noexcept;

  public:
    MountFlagsBuilder &withFlagsFrom(const MountMode &md) noexcept;
    MountFlagsBuilder &withRemount() noexcept;
    int build() const noexcept;
};