c  read HADES unformatted acceptance matrix for theorists
c
c  07/04/05 R. Holzmann
c
      program readHAFT

      implicit none
      character*60 fname
      character*80 comment
      integer*4 xdim, ydim, zdim
      real*4 pmin,pmax,thmin,thmax,phmin,phmax
      real*4 matrix(100,45,30)
      common /haft/ xdim,ydim,zdim,pmin,pmax,thmin,thmax,phmin,phmax,
     +              matrix
      integer*4 ix,iy,iz
      real*4 p,theta,phi,val,getHadesAcceptance

      write(6,'('' Enter file name: '',$)')
      read(5,'(a60)') fname

      open(unit=10,file=fname,form='unformatted',err=100)
      read(10) comment
      read(10) xdim, ydim, zdim
      read(10) pmin, pmax, thmin, thmax, phmin, phmax
      read(10) matrix

      write(6,'('' coms= '',a80)') comment
      write(6,*) 'dims= ',xdim, ' ', ydim, ' ', zdim
      write(6,*) 'lims= ',pmin, ' ', pmax, ' ', thmin, ' ', thmax,
     +           ' ', phmin, ' ', phmax
      write(6,*) 'size of matrix :', xdim*ydim*zdim

      close(10)

 10   write(6,'(''Enter ix,iy,iz: '',$)')
c       read(5,*) ix,iy,iz
c       if(ix.le.0) stop
c       write(6,*) 'val= ',matrix(ix,iy,iz)
        write(6,'(''Enter p, theta, phi: '',$)')
        read(5,*) p,theta,phi
        if (p.lt.0.) stop
        val=getHadesAcceptance(0,p,theta,phi)
        write(6,*) 'val= ', val
      goto 10  ! oh, how I love good old Fortran gotos!

      stop

 100  write(6,*) 'Cannot open file!'
      stop
      end

c
c  Returns HADES acceptance for particle id of given momentum (MeV/c),
c  theta (deg.) and phi (deg.) by simple table lookup.
c 
      real*4 function getHadesAcceptance(id,p0,theta0,phi0)
      implicit none
      integer*4 id
      real*4 p0,theta0,phi0
      real*4 p,theta,phi
      integer*4 xdim, ydim, zdim
      real*4 pmin,pmax,thmin,thmax,phmin,phmax
      real*4 matrix(100,45,30)
      integer*4 ix,iy,iz
      common /haft/ xdim,ydim,zdim,pmin,pmax,thmin,thmax,phmin,phmax,
     +              matrix

      getHadesAcceptance = 0.
      p = p0
      theta = theta0
      phi = phi0

      if (p.lt.pmin .or. theta.lt.thmin .or. theta.gt.thmax) return
      if (phi. gt. phmax) phi = mod(phi,phmax) ! modulo sector
      if (p.gt.pmax) p = pmax ! level off acceptance

      ix = xdim*(p-pmin)/(pmax-pmin) + 1       ! compute table indexes
      iy = ydim*(theta-thmin)/(thmax-thmin) + 1
      iz = zdim*(phi-phmin)/(phmax-phmin) + 1

      write(6,*) p, theta, phi, " -> ", ix, iy, iz

      getHadesAcceptance = matrix(ix,iy,iz)    ! look up acceptance 

      return
      end
