# Wrapper for Python compile worker

PY_CC_PATH = compile.py
PY_EXEC = python3

all:
	$(PY_EXEC) $(PY_CC_PATH) os_all

build:
	$(PY_EXEC) $(PY_CC_PATH) os_build

iso:
	$(PY_EXEC) $(PY_CC_PATH) os_iso

run:
	$(PY_EXEC) $(PY_CC_PATH) os_run

clean:
	$(PY_EXEC) $(PY_CC_PATH) os_clean