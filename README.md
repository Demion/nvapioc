### Changelog:

- **v0.3** - [NvApiOc v0.3](https://github.com/Demion/nvapioc/releases/download/v0.3/NvApiOc_v0.3.zip)
  * Add require administrator rights (UAC).
  * Add x64 build support.
- **v0.2** - [NvApiOc v0.2](https://github.com/Demion/nvapioc/releases/download/v0.2/NvApiOc_v0.2.zip)
  * Add log file.
  * Add TCC devices support.
  * Add restart driver option.
  * Remove PState options *(not working as expected)*.
- **v0.1** - [NvApiOc v0.1](https://github.com/Demion/nvapioc/releases/download/v0.1/NvApiOc_v0.1.zip)

### About:

NvApiOc Open Source NvAPI NVIDIA GPU Overclock
  
### Usage:

- **-core** gpuIndex pState frequencyOffsetKHz *(0 = default)*
- **-mem** gpuIndex pState frequencyOffsetKHz *(0 = default)*
- **-volt** gpuIndex voltageUV *(0 = default)*
- **-power** gpuIndex power
- **-temp** gpuIndex priority tempC *(0 = false 1 = true)*
- **-fan** gpuIndex fanIndex level *(-1 = auto)*
- **-led** gpuIndex type brightness *(0 = logo 1 = sliBridge)*
- **-restart**

### Donation:

- ETH: 0xcb4effdeb46479caa0fef5f5e3569e4852f753a2
- BTC: 1H1zNLHNxqtMgVYJESF6PjPVq2h9tLW4xG

### Credits:

- NVIDIA Inspector – Tool (Orbmu2k) https://orbmu2k.de/tools/nvidia-inspector-tool
- nvapi-rs - NVAPI bindings for Rust (arcnmx) https://github.com/arcnmx/nvapi-rs https://arcnmx.github.io/nvapi-rs/nvapi/
