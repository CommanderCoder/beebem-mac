# Simple instructions #

The submodule should already exist, but you can get it directly with:

```
git clone https://github.com/create-dmg/create-dmg.git
```

## XCODE ##

_Product->Build for Profiling..._

Find the output folder _Product->Show Build Folder In Finder_  and find the **Release** folder
e.g.
```
../Project/Build/Products/Release
```

Lastly run the <u>create-distribution.sh</u> script
```
./create-distribution.sh ../Project/Build/Products/Release
```

**Important:** Don't forget to [notarize your DMG](https://stackoverflow.com/a/60800864/64949).

[Apple Docs](https://developer.apple.com/documentation/security/notarizing_macos_software_before_distribution/customizing_the_notarization_workflow#3087734)


Need applid, team id, notarytool password,  etc.
