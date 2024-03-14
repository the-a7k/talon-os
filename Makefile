# Wrapper for make.py

all:
	python3 make.py os_all

build:
	python3 make.py os_build

iso:
	python3 make.py os_iso

run:
	python3 make.py os_run

clean:
	python3 make.py os_clean