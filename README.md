GTimeUtils
=========

**A collection of simple, small time utilities.**

From version 2.0 onwards, both GTimer and GStopwatch are copies from GNOME Clocks' respective stack
pages. Prior to version 2.0, both utilities served more as a practice than anything else. Since I
like the modern look of GNOME Clocks, but dislike the fact that all its functionality is combined
into one application I have re-purposed its ui files into standalone utilities.

Installation
------------

All utilities in GTimeUtils are written using GTK+ 3. For playing sound with notifications, you
need to have `gsound` installed. To build, simply run these commands:

	$ ./autogen.sh
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

