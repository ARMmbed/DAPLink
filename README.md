## DAPLink interface firmware update microsite

[![Build Status](https://travis-ci.org/ARMmbed/DAPLink.svg?branch=gh-pages)](https://travis-ci.org/ARMmbed/DAPLink)

#### Updating the microsite content
1. Checkout the DAPLink `gh-pages` branch
1. [Download the latest release package](https://github.com/armmbed/daplink/releases/latest)
1. Extract files into `./firmware/`
1. Inside `update.yml` modify the `fw_version` field
1. Look for any new builds and add entries to `update.yml`
1. Commit chamges and make a pull request against the `gh-pages` branch
