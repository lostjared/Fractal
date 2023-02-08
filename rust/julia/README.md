To use:

Accepted Arguments:
        
    --output [output file] required: true
    --param [complex pair] required: true
    --res [output resolution] required: true
    --iter [number of iterations] required: true

Example:

    $ cargo run --release -- --output=test.png --res=640,480 --iter=500 --param=0.355,0.355
