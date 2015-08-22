GTimeUtils
=========

**A collection of simple, small time utilities.**

Usage
-----

### GTimer

GTimer can take four (five if you count `--help`) command line arguments:

	-s, --seconds     Specify seconds to count down from
	-m, --minutes     Specify minutes to count down from
	-u, --hours       Specify hours to count down from
	-t, --text        Set an alternative notification text
	-r, --run         Immediately start the countdown

Installation
------------

All utilities in GTimeUtils are written using GTK+ 3. For sending notifications, you need to have
libcanberra and libnotify installed. To build, simply run these commands:

	$ ./autogen
	$ make
	# make clean install

Translations
------------

You can help translate GTimeUtils to your language! To do so, simply follow these steps:

	$ cd po
	$ intltool-update --pot
	$ mv gtimeutils.pot <language code>.po

Where `<language code>` is the code of your language (e.g. `nl` for Dutch, `fr` for French, `en_GB` for British English...).
Edit the [LINGUAS](https://github.com/Unia/gtimeutils/blob/master/po/LINGUAS) file and add your language code. Please keep the list sorted alphabetically.
Lastly, open the `.po` file you just generated and translate all the strings. Don't forget to fill in the information in the header!

When a translation needs updating, execute the following commands:

	$ cd po
	$ intltool-update --pot
	$ intltool-update --dist --gettext-package=gtimeutils --output-file=<language code>_new.po <language code>

Then make the necessary changes and overwrite the old `.po` file:

	$ mv <language code>_new.po <language code>.po

Bugs
----

For any bug or request [fill an issue](https://github.com/Unia/gtimeutils/issues) on [GitHub][github].

License
-------

Please see [LICENSE](https://github.com/Unia/gtimeutils/blob/master/LICENSE) on [GitHub][github].

**Copyright © 2015** Jente Hidskes &lt;hjdskes@gmail.com&gt;

  [github]: https://github.com/Unia/gtimeutils

