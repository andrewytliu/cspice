LIBPKGS = lib
MAIN    = main

LIBS    = $(addprefix -l, $(LIBPKGS))

all: libs main

libs:
	@for pkg in $(LIBPKGS); \
	do \
		echo "Checking $$pkg..."; \
		cd $$pkg; make --no-print-directory PKGNAME=$$pkg; \
		cd ..; \
	done

main:
	@echo "Checking $(MAIN)..."
	@cd $(MAIN); \
		make --no-print-directory INCLIB="$(LIBS)";
	@ln -fs bin/$(EXEC) .

clean:
	@for pkg in $(LIBPKGS); \
	do \
		echo "Cleaning $$pkg..."; \
		cd $$pkg; make --no-print-directory PKGNAME=$$pkg clean; \
		cd ..; \
	done
	@echo "Cleaning $(MAIN)..."
	@cd $(MAIN); make --no-print-directory clean
	@echo "Removing $(EXEC)..."
	@rm -f bin/$(EXEC)

