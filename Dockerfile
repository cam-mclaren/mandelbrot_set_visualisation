from alpine

copy . /mandelbrot_set_visualisation

run apk add build-base
run apk add gmp-dev
run apk add mpfr-dev


