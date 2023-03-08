# aja-play
Simple Video Playout software using libavcodec and AJA DirectShow filters

# prerequisites
- [libavcodec DirectShow filters](https://github.com/Nevcairiel/LAVFilters)
- [AJA DirectShow filters](https://www.aja.com/support/directshow)
- Visual Studio
- Windows SDK

# building
1. Install the AJA driver for your card(s).
2. Unzip the AJA DirectShowPackage file.
   - Register the filters with regsvr32
   - Place the include files in a folder named aja inside the project root
3. Install the libavcodec filters.
4. Open & Build aja-play.sln
