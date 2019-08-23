c  Declaration of acceptance matrix arrays
c
c  The dimensions MUST match the array sizes in the file!
c
      integer*4 size
      parameter (size=250000) ! <<== change if necessary
c
      character*80 fname, fname2
      character*80 comment, comment2
      integer*4 xdim, ydim, zdim
      real*4 dx, dy, dz
      real*4 pmin, pmax, thmin, thmax, phmin, phmax
      real*4 matrix2(size) ! matrices are declared for
      real*4 matrix3(size) ! e+, e-, pi+, pi-, K+, K- and p
      real*4 matrix8(size)
      real*4 matrix9(size)
      real*4 matrix10(size)
      real*4 matrix12(size)
      real*4 matrix14(size)
      integer*4 iseed
      real*4 sigpA(3), sigpB(3), sigth, sigph, XX0
      integer*4 xdim2, ydim2, zdim2
      real*4 dx2, dy2, dz2
      real*4 mmin, mmax, ptmin, ptmax, rapmin, rapmax
      real*4 matrix51(size)
      common /haft/ fname, comment, xdim, ydim, zdim, dx, dy, dz,
     +              pmin, pmax, thmin, thmax, phmin, phmax,
     +              matrix2, matrix3, matrix8, matrix9,
     +              matrix10, matrix12, matrix14,
     +              iseed, sigpA, sigpB, sigth, sigph, XX0,
     +              fname2, comment2, xdim2, ydim2, zdim2,
     +              dx2, dy2, dz2,
     +              mmin, mmax, ptmin, ptmax, rapmin, rapmax,
     +              matrix51