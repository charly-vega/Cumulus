# Cumulus
Cumulus is a **free**, open-source replacement for [CloudApp](http://www.getcloudapp.com/) that **utilizes your own S3 for storage**.

![Screenshot](https://raw.github.com/nrj/Cumulus/master/Screenshots/notification.png)

![Screenshot](https://raw.github.com/nrj/Cumulus/master/Screenshots/settings.png)

# Download
You can download the latest version [here](https://raw.github.com/nrj/Cumulus/master/Builds/Cumulus.dmg)

# Change the default screenshot directory

_This is optional._ By default OS X puts screenshots on your Desktop. But if you'd rather they go somewhere else you can change the location in Terminal with following:

    $ mkdir -p ~/Pictures/Screenshots
    $ defaults write com.apple.screencapture location ~/Pictures/Screenshots
    $ killall SystemUIServer

# Building
If you you have the latest version of of [Xcode](https://itunes.apple.com/us/app/xcode/id497799835?mt=12) and [Cocoapods](http://cocoapods.org) you can build Cumulus yourself with the following steps:

    $ git clone git@github.com:nrj/Cumulus.git
    $ cd Cumulus
    $ pod setup
    $ pod install
    $ open Cumulus.xcworkspace
    

Then hit Build & Run.

# Contributing

If you would like to contribute to this project, please fork from the `master` branch and send me a pull request.

# License

Cumulus is under the MIT License. The cloud icon was created by the [Aha-Soft Team](http://www.aha-soft.com/) and is under the [Creative Commons License](http://creativecommons.org/licenses/by/3.0/legalcode). See LICENSE for more details.