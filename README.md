<p align="center">
  <a href="https://github.com/profLewis/librat">
    <img src="http://www2.geog.ucl.ac.uk/~plewis/images/millet/millet.gif" alt="Millet the movie" width="72" height="72">
  </a>
</p>
<h2 align="center">librat 🐀</h2>

<p align="center">
  A ray tracing library built for radiative transfer simulations for remote sensing studies.
  <br>
  <a href="https://github.com/profLewis/librat/docs"><strong>Explore librat docs »</strong></a>
  <br>
  <br>
  <a href="https://github.com/profLewis/librat/issues/new?template=bug.md">Report bug</a>
  ·
  <a href="https://github.com/profLewis/librat/issues/new?template=feature.md&labels=feature">Request feature</a>
  ·
  <a href="https://blog.librat.com/">Blog</a>
</p>

## Table of contents

- [Quick start](#quick-start)
- [Status](#status)
- [What's included](#whats-included)
- [Bugs and feature requests](#bugs-and-feature-requests)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Community](#community)
- [Versioning](#versioning)
- [Creators](#creators)
- [Thanks](#thanks)
- [Copyright and license](#copyright-and-license)


## Quick start

Several quick start options are available:

- [Download the latest release.](https://github.com/twbs/bootstrap/archive/v4.3.1.zip)
- Clone the repo: `git clone https://github.com/profLewis/librat.git`

`librat` (actually called `ratlib` in the code ) is a ray tracing library built for radiative transfer simulations for remote sensing studies. It was originally written in the later 1980s/early 1990s by [P. Lewis](http://www2.geog.ucl.ac.uk/~plewis/), as part of his [PhD, A Botanical Plant Modelling System for Remote Sensing
Simulation Studies, UCL, 1996](http://www2.geog.ucl.ac.uk/~plewis/phd). The main advantage of this code is that it is part of a community reference effort for radiative transfer. That has been a major motivating factor in releasing the code in this (easy to use) form. It has also been used in a [wide range of studies](docs/ratindex.md).

Read the [Getting started page](https://github.com/profLewis/librat/docs/getting-started/introduction/) for information on the examples, and more.


## Status

[![Slack](https://bootstrap-slack.herokuapp.com/badge.svg)](https://bootstrap-slack.herokuapp.com/)
[![Build Status](https://github.com/twbs/bootstrap/workflows/Tests/badge.svg)](https://github.com/twbs/bootstrap/actions?workflow=Tests)
[![npm version](https://img.shields.io/npm/v/bootstrap.svg)](https://www.npmjs.com/package/bootstrap)
[![Gem version](https://img.shields.io/gem/v/bootstrap.svg)](https://rubygems.org/gems/bootstrap)
[![Meteor Atmosphere](https://img.shields.io/badge/meteor-twbs%3Abootstrap-blue.svg)](https://atmospherejs.com/twbs/bootstrap)
[![Packagist Prerelease](https://img.shields.io/packagist/vpre/twbs/bootstrap.svg)](https://packagist.org/packages/twbs/bootstrap)
[![NuGet](https://img.shields.io/nuget/vpre/bootstrap.svg)](https://www.nuget.org/packages/bootstrap/absoluteLatest)
[![peerDependencies Status](https://img.shields.io/david/peer/twbs/bootstrap.svg)](https://david-dm.org/twbs/bootstrap?type=peer)
[![devDependency Status](https://img.shields.io/david/dev/twbs/bootstrap.svg)](https://david-dm.org/twbs/bootstrap?type=dev)
[![Coverage Status](https://img.shields.io/coveralls/github/twbs/bootstrap/master.svg)](https://coveralls.io/github/twbs/bootstrap?branch=master)
[![CSS gzip size](https://img.badgesize.io/twbs/bootstrap/master/dist/css/bootstrap.min.css?compression=gzip&label=CSS+gzip+size)](https://github.com/twbs/bootstrap/tree/master/dist/css/bootstrap.min.css)
[![JS gzip size](https://img.badgesize.io/twbs/bootstrap/master/dist/js/bootstrap.min.js?compression=gzip&label=JS+gzip+size)](https://github.com/twbs/bootstrap/tree/master/dist/js/bootstrap.min.js)
[![BrowserStack Status](https://www.browserstack.com/automate/badge.svg?badge_key=SkxZcStBeExEdVJqQ2hWYnlWckpkNmNEY213SFp6WHFETWk2bGFuY3pCbz0tLXhqbHJsVlZhQnRBdEpod3NLSDMzaHc9PQ==--3d0b75245708616eb93113221beece33e680b229)](https://www.browserstack.com/automate/public-build/SkxZcStBeExEdVJqQ2hWYnlWckpkNmNEY213SFp6WHFETWk2bGFuY3pCbz0tLXhqbHJsVlZhQnRBdEpod3NLSDMzaHc9PQ==--3d0b75245708616eb93113221beece33e680b229)
[![Backers on Open Collective](https://img.shields.io/opencollective/backers/bootstrap.svg)](#backers)
[![Sponsors on Open Collective](https://img.shields.io/opencollective/sponsors/bootstrap.svg)](#sponsors)


## What's included

Within the download you'll find the following directories and files, logically grouping common assets and providing both compiled and minified variations. You'll see something like this:

```text
bootstrap/
└── dist/
    ├── css/
    │   ├── bootstrap-grid.css
    │   ├── bootstrap-grid.css.map
    │   ├── bootstrap-grid.min.css
    │   ├── bootstrap-grid.min.css.map
    │   ├── bootstrap-reboot.css
    │   ├── bootstrap-reboot.css.map
    │   ├── bootstrap-reboot.min.css
    │   ├── bootstrap-reboot.min.css.map
    │   ├── bootstrap-utilities.css
    │   ├── bootstrap-utilities.css.map
    │   ├── bootstrap-utilities.min.css
    │   ├── bootstrap-utilities.min.css.map
    │   ├── bootstrap.css
    │   ├── bootstrap.css.map
    │   ├── bootstrap.min.css
    │   └── bootstrap.min.css.map
    └── js/
        ├── bootstrap.bundle.js
        ├── bootstrap.bundle.js.map
        ├── bootstrap.bundle.min.js
        ├── bootstrap.bundle.min.js.map
        ├── bootstrap.esm.js
        ├── bootstrap.esm.js.map
        ├── bootstrap.esm.min.js
        ├── bootstrap.esm.min.js.map
        ├── bootstrap.js
        ├── bootstrap.js.map
        ├── bootstrap.min.js
        └── bootstrap.min.js.map
```

We provide compiled CSS and JS (`bootstrap.*`), as well as compiled and minified CSS and JS (`bootstrap.min.*`). [source maps](https://developers.google.com/web/tools/chrome-devtools/javascript/source-maps) (`bootstrap.*.map`) are available for use with certain browsers' developer tools. Bundled JS files (`bootstrap.bundle.js` and minified `bootstrap.bundle.min.js`) include [Popper](https://popper.js.org/).


## Bugs and feature requests

Have a bug or a feature request? Please first read the [issue guidelines](https://github.com/twbs/bootstrap/blob/master/.github/CONTRIBUTING.md#using-the-issue-tracker) and search for existing and closed issues. If your problem or idea is not addressed yet, [please open a new issue](https://github.com/twbs/bootstrap/issues/new).


## Documentation

Bootstrap's documentation, included in this repo in the root directory, is built with [Hugo](https://gohugo.io/) and publicly hosted on GitHub Pages at <https://getbootstrap.com/>. The docs may also be run locally.

Documentation search is powered by [Algolia's DocSearch](https://community.algolia.com/docsearch/). Working on our search? Be sure to set `debug: true` in `site/assets/js/src/search.js` file.

### Running documentation locally

1. Run `npm install` to install the Node.js dependencies, including Hugo (the site builder).
2. Run `npm run test` (or a specific npm script) to rebuild distributed CSS and JavaScript files, as well as our docs assets.
3. From the root `/bootstrap` directory, run `npm run docs-serve` in the command line.
4. Open `http://localhost:9001/` in your browser, and voilà.

Learn more about using Hugo by reading its [documentation](https://gohugo.io/documentation/).

### Documentation for previous releases

You can find all our previous releases docs on <https://getbootstrap.com/docs/versions/>.

[Previous releases](https://github.com/twbs/bootstrap/releases) and their documentation are also available for download.


## Contributing

Please read through our [contributing guidelines](https://github.com/twbs/bootstrap/blob/master/.github/CONTRIBUTING.md). Included are directions for opening issues, coding standards, and notes on development.

Moreover, if your pull request contains JavaScript patches or features, you must include [relevant unit tests](https://github.com/twbs/bootstrap/tree/master/js/tests). All HTML and CSS should conform to the [Code Guide](https://github.com/mdo/code-guide), maintained by [Mark Otto](https://github.com/mdo).

Editor preferences are available in the [editor config](https://github.com/twbs/bootstrap/blob/master/.editorconfig) for easy use in common text editors. Read more and download plugins at <https://editorconfig.org/>.


## Community

Get updates on Bootstrap's development and chat with the project maintainers and community members.

- Follow [@getbootstrap on Twitter](https://twitter.com/getbootstrap).
- Read and subscribe to [The Official Bootstrap Blog](https://blog.getbootstrap.com/).
- Join [the official Slack room](https://bootstrap-slack.herokuapp.com/).
- Chat with fellow Bootstrappers in IRC. On the `irc.freenode.net` server, in the `##bootstrap` channel.
- Implementation help may be found at Stack Overflow (tagged [`bootstrap-4`](https://stackoverflow.com/questions/tagged/bootstrap-4)).
- Developers should use the keyword `bootstrap` on packages which modify or add to the functionality of Bootstrap when distributing through [npm](https://www.npmjs.com/browse/keyword/bootstrap) or similar delivery mechanisms for maximum discoverability.


## Versioning

For transparency into our release cycle and in striving to maintain backward compatibility, Bootstrap is maintained under [the Semantic Versioning guidelines](https://semver.org/). Sometimes we screw up, but we adhere to those rules whenever possible.

See [the Releases section of our GitHub project](https://github.com/twbs/bootstrap/releases) for changelogs for each release version of Bootstrap. Release announcement posts on [the official Bootstrap blog](https://blog.getbootstrap.com/) contain summaries of the most noteworthy changes made in each release.


## Creators

**Mark Otto**

- <https://twitter.com/mdo>
- <https://github.com/mdo>

**Jacob Thornton**

- <https://twitter.com/fat>
- <https://github.com/fat>


## Thanks

<a href="https://www.browserstack.com/">
  <img src="https://live.browserstack.com/images/opensource/browserstack-logo.svg" alt="BrowserStack Logo" width="192" height="42">
</a>

Thanks to [BrowserStack](https://www.browserstack.com/) for providing the infrastructure that allows us to test in real browsers!


## Sponsors

Support this project by becoming a sponsor. Your logo will show up here with a link to your website. [[Become a sponsor](https://opencollective.com/bootstrap#sponsor)]

[![](https://opencollective.com/bootstrap/sponsor/0/avatar.svg)](https://opencollective.com/bootstrap/sponsor/0/website)
[![](https://opencollective.com/bootstrap/sponsor/1/avatar.svg)](https://opencollective.com/bootstrap/sponsor/1/website)
[![](https://opencollective.com/bootstrap/sponsor/2/avatar.svg)](https://opencollective.com/bootstrap/sponsor/2/website)
[![](https://opencollective.com/bootstrap/sponsor/3/avatar.svg)](https://opencollective.com/bootstrap/sponsor/3/website)
[![](https://opencollective.com/bootstrap/sponsor/4/avatar.svg)](https://opencollective.com/bootstrap/sponsor/4/website)
[![](https://opencollective.com/bootstrap/sponsor/5/avatar.svg)](https://opencollective.com/bootstrap/sponsor/5/website)
[![](https://opencollective.com/bootstrap/sponsor/6/avatar.svg)](https://opencollective.com/bootstrap/sponsor/6/website)
[![](https://opencollective.com/bootstrap/sponsor/7/avatar.svg)](https://opencollective.com/bootstrap/sponsor/7/website)
[![](https://opencollective.com/bootstrap/sponsor/8/avatar.svg)](https://opencollective.com/bootstrap/sponsor/8/website)
[![](https://opencollective.com/bootstrap/sponsor/9/avatar.svg)](https://opencollective.com/bootstrap/sponsor/9/website)


## Backers

Thank you to all our backers! 🙏 [[Become a backer](https://opencollective.com/bootstrap#backer)]

[![Backers](https://opencollective.com/bootstrap/backers.svg?width=890)](https://opencollective.com/bootstrap#backers)


## Copyright and license

Code and documentation copyright 2011-2020 the [Bootstrap Authors](https://github.com/twbs/bootstrap/graphs/contributors) and [Twitter, Inc.](https://twitter.com) Code released under the [MIT License](https://github.com/twbs/bootstrap/blob/master/LICENSE). Docs released under [Creative Commons](https://creativecommons.org/licenses/by/3.0/).



To set the library up on your computer, you need to configure and compile the code. As explained in more detail below, this involves typing:

    ./configure; make clean all test
    
at the shell prompt. Press the `<return>` key to execute the commands. The semicolon `;` allows us to put multiple commands on the same line. We could otherwise type them on seperate lines. 


<h4 align="center">📲Configuration</h4>



To configure this library on your computer, in a unix shell type:

    ./configure

Press the `<return>` key to execute the command. The configure script sets up the environment for your computer. The reason we type `./` in front of it, it to tell the unix interpreter that the script `configure` is in the **current working directory** which is indicated by `.`. It does this by testing that various options work on your particular setup. The range of options we have should work with most `*nix` setups. Specifically, [`configure`](configure)  sets variables from [`config.in`](config.in):

    set CCS = ("gcc" "cc")
    set CFLAGSS = ("-I." "-fPIC")
    set DYNS = ("ld -G" "libtool  -L.. -lc -dynamic -undefined dynamic_lookup")
    set MAKES = ("gmake" "make")
    set OPTS = ("-g")
    # specify a temporary directory
    set TMP = /tmp
    
These are the options considered in configure. For example, we first try a compiler `gcc` and if that fails, try `cc`. We try ther dynamic linker `ld -G` and if that fails, `libtool  -L.. -lc -dynamic -undefined dynamic_lookup` etc. Note that the configutaion will settle on the first option in the list that passes some internal tests (follow through the script [`configure`](configure)).

The configuation the sets e.g. (settings on `OS X x86_64`):

    ARCH = x86_64
    OBJ = objects.${ARCH}

    BPMS = /Users/plewis/librat
    LIB = ${BPMS}/lib
    BIN = ${BPMS}/bin

    CFLAGS = -I.
    MAKE = make
    WG = -L -R --create-dirs -o
    WGET = /usr/bin/curl
    CC = gcc
    DYN = libtool -L.. -lc -dynamic -undefined dynamic_lookup
    OPT = -g

in the file [`src/makefile`](src/makefile). Note that [`src/makefile`](src/makefile) is generate from the template [`src/Makefile.in`](src/Makefile.in).


<h4 align="center">⏳ Compilation</h4>


To compile and test the library (after running [`./configure`)](configure), type:

    make clean all test
    
Press the `<return>` key to execute the command. In fact, if you forget to run [`./configure`)](configure), this will run it for you, via the file [`makefile`](makefile). The command `make clean` clears out any previously compiled code. Then `make all` compiles the library `${BPMS}/lib/${ARCH}/libratlib.so` (so, you refer to this as `-L ${BPMS}/lib/${ARCH} -lratlib`. It also compiles a `C` language interface to the library, `${BPMS}/bin/${ARCH}/start`. `make test` runs a test on the `start` code, comparing a simple ray tracing outout to a reference one.

To sum up, after compilation, you will have generated files in

* [`bin/${ARCH}`](bin/x86_64) : executable -- `start`
* [`lib/${ARCH}`](lib/x86_64) : shared object library -- `libratlib.so`

Notice that these links ⛓ (`bin/x86_64`, `lib/x86_64`)  don't exist on the github repository: they are user-generated!

