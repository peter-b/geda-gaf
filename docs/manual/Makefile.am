info_TEXINFOS = lepton-manual.texi
lepton_manual_TEXINFOS = \
	attribute-net.texi \
	attributes.texi \
	communication.texi \
	configuration.texi \
	development.texi \
	geda-file-format-spec.texi \
	index.texi \
	installation.texi \
	lepton-attrib.texi \
	lepton-cli-config.texi \
	lepton-cli-export.texi \
	lepton-cli-shell.texi \
	lepton-cli.texi \
	lepton-eda-fonts.texi \
	lepton-netlist.texi \
	lepton-netlist-allegro.texi \
	lepton-netlist-bom.texi \
	lepton-netlist-cascade.texi \
	lepton-netlist-pads.texi \
	lepton-netlist-switcap.texi \
	lepton-schematic.texi \
	logging.texi \
	no-connect-symbol.texi \
	overview.texi \
	placeholder.texi \
	symbol-creation.texi \
	symbol-editing.texi \
	symbol-version-attribute.texi \
	symbols.texi \
	title-block-symbol.texi

AM_MAKEINFOHTMLFLAGS = --css-ref=lepton-manual.css

dvi: #do nothing for 'make dvi'

schematics = \
	analysis.sch \
	clocks.sch \
	ckt.sch \
	coordinate-space.sch \
	net-attribute.sch \
	path-example.sch \
	rf_cascade.sch \
	text-layout.sch

schematics_pdf = ${schematics:.sch=.pdf}
schematics_png = ${schematics:.sch=.png}

symbols = \
	7400-1.sym \
	7400-1-hidden.sym

symbols_pdf = ${symbols:.sym=.pdf}
symbols_png = ${symbols:.sym=.png}

PICTURES = \
	$(schematics) \
	$(schematics_pdf) \
	$(schematics_png) \
	$(symbols) \
	$(symbols_pdf) \
	$(symbols_png)

infoimagesdir = $(infodir)
infoimages_DATA = $(schematics_png) $(symbols_png)

# lepton-cli export --no-color -k 0,5cm -o coordinate-space.png coordinate-space.sch
# lepton-cli export --no-color -o coordinate-space.pdf coordinate-space.sch
# lepton-cli export -k 1cm -o text-layout.png text-layout.sch
# lepton-cli export -m 0,1cm -p iso_a8 -o text-layout.pdf text-layout.sch
# lepton-cli export -k 1cm -o path-example.png path-example.sch
# lepton-cli export -o path-example.pdf path-example.sch
# lepton-cli export -m 0 -F Sans -o 7400-1-hidden.pdf 7400-1-hidden.sym
# lepton-cli export -m 0 -F Sans -o 7400-1.pdf 7400-1.sym
# lepton-cli export -m 0 -F Sans -k 2mm -o rf_cascade.png rf_cascade.sch
# lepton-cli export -m 0 -F Sans -p iso_a6 -o rf_cascade.pdf rf_cascade.sch
noinst_DATA = $(PICTURES)

EXTRA_DIST = \
	$(PICTURES) \
	example.cas \
	test.ana \
	lepton-manual.css \
	scm-component.scm \
	cmd-component.sh

all: all-am html
	$(MKDIR_P) $(builddir)/lepton-manual.html/
	cp -fv $(srcdir)/lepton-manual.css $(builddir)/lepton-manual.html/
	for file in $(schematics_png) $(symbols_png); \
	  do cp $(srcdir)/$$file ${builddir}/lepton-manual.html/; \
	done

install-data-local: install-html
