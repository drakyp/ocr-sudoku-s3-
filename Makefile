SUBDIRS := cut filters solver xor

.PHONY: all $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	$(RM) -r $(SUBDIRS:%=%/build)

.PHONY: all clean
