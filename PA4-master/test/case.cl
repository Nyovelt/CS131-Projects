
(*  Example cool program testing as many aspects of the code generator
    as possible.
 *)

class Main inherits IO {
  main():Object { 
    {
            (new IO).out_int(1);
            (let countdown : Int <- 20 in
                -- while 0 < countdown loop
                --     {
                --         (new IO).out_int(countdown);
                --         -- cells.evolve();
                --         -- cells.print();
                --         countdown <- countdown - 1;
                --     }
                -- pool
              (new IO).out_int(countdown)
            );
    }
    };
};
