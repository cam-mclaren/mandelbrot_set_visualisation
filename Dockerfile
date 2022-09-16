FROM alpine

VOLUME . /mandelbrot_set_visualisation

RUN apk add build-base \
&& apk add gmp-dev \
&& apk add mpfr-dev


