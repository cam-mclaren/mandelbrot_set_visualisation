FROM alpine

VOLUME . /mandelbrot_set_visualisation

RUN apk add build-base
RUN apk add gmp-dev
RUN apk add mpfr-dev


