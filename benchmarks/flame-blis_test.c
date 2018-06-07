#include "chayai.h"

#include <unistd.h>
#include "blis.h"

BENCHMARK(flame, blis, 10, 1) {
    // based on test/test_hemm.c
    obj_t a, b, c;
    obj_t c_save;
    obj_t alpha, beta;
    dim_t m, n;
    dim_t p;
    dim_t p_begin, p_end, p_inc;
    int   m_input, n_input;
    num_t dt;
    int   r, n_repeats;
    side_t side;
    uplo_t uploa;
    f77_char f77_side;
    f77_char f77_uploa;

    //bli_init();

    bli_error_checking_level_set( BLIS_NO_ERROR_CHECKING );

    n_repeats = 3;

    p_begin = 20;
    p_end   = 200;
    p_inc   = 20;

    m_input = -1;
    n_input = -1;

	dt = BLIS_DCOMPLEX;

    side = BLIS_LEFT;

    uploa = BLIS_LOWER;

    bli_param_map_blis_to_netlib_side( side, &f77_side );
    bli_param_map_blis_to_netlib_uplo( uploa, &f77_uploa );

    for ( p = p_begin; p <= p_end; p += p_inc )
    {
        if ( m_input < 0 ) m = p * ( dim_t )abs(m_input);
        else               m =     ( dim_t )    m_input;
        if ( n_input < 0 ) n = p * ( dim_t )abs(n_input);
        else               n =     ( dim_t )    n_input;

        bli_obj_create( dt, 1, 1, 0, 0, &alpha );
        bli_obj_create( dt, 1, 1, 0, 0, &beta );

        if ( bli_is_left( side ) )
            bli_obj_create( dt, m, m, 0, 0, &a );
        else
            bli_obj_create( dt, n, n, 0, 0, &a );
        bli_obj_create( dt, m, n, 0, 0, &b );
        bli_obj_create( dt, m, n, 0, 0, &c );
        bli_obj_create( dt, m, n, 0, 0, &c_save );

        bli_randm( &a );
        bli_randm( &b );
        bli_randm( &c );

        bli_obj_set_struc( BLIS_HERMITIAN, a );
        bli_obj_set_uplo( uploa, a );

        // Randomize A, make it densely Hermitian, and zero the unstored
        // triangle to ensure the implementation reads only from the stored
        // region.
        bli_randm( &a );
        bli_mkherm( &a );
        bli_mktrim( &a );

        bli_setsc(  (2.0/1.0), 1.0, &alpha );
        bli_setsc( -(1.0/1.0), 0.0, &beta );

        bli_copym( &c, &c_save );

        for ( r = 0; r < 5; ++r )
        {
            bli_copym( &c_save, &c );

            bli_hemm( side,
			          &alpha,
			          &a,
			          &b,
			          &beta,
			          &c );
        }

        bli_obj_free( &alpha );
        bli_obj_free( &beta );

        bli_obj_free( &a );
        bli_obj_free( &b );
        bli_obj_free( &c );
        bli_obj_free( &c_save );
    }

    //bli_finalize();
}

int main(int argc, char** argv) {

    REGISTER_BENCHMARK(flame, blis); // high-performance dense linear algebra

    RUN_BENCHMARKS(argc, argv);

    return 0;
}