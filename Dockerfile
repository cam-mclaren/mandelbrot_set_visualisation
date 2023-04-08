FROM alpine


RUN apk add build-base \
&& apk add gmp-dev \
&& apk add mpfr-dev


