# Wrapper for compile.py

all:
	python3 compile.py os_all

build:
	python3 compile.py os_build

iso:
	python3 compile.py os_iso

run:
	python3 compile.py os_run

clean:
	python3 compile.py os_clean