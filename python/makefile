PROJ=protos

venv:
	python3 -m venv venv
	venv/bin/python3 -m pip install --upgrade pip
	venv/bin/python3 -m pip install --upgrade setuptools wheel
	venv/bin/python3 -m pip install flake8
	venv/bin/python3 -m pip install mypy

.PHONY: clean
clean:
	rm -rf .mypy_cache protos/__pycache__ tests/__pycache__ venv

.PHONY: lint
lint: venv
	./venv/bin/python -m mypy ./$(PROJ)
	./venv/bin/python -m flake8 --max-line-length=83 ./$(PROJ) ./tests

.PHONY: test
test: venv
	./venv/bin/python3 -m unittest discover
