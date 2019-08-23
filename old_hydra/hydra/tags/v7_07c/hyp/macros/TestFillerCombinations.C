// This MACRO is for out-of-the-box testing for
// new particle combinations for the filler.
// Just add all the particles you want and
// check the output... --- Björn
// Problems? Contact: bjoern.spruck@physik.uni-giessen.de

{
	gROOT->Reset();
	gSystem->Load("mydev/lib/libHyp.so");
	
	filler=new HHypListFiller("test","test");

	filler->AddTrack(HPidPhysicsConstants::pid("e+"));
	filler->AddTrack(HPidPhysicsConstants::pid("e-"));
	filler->AddTrack(HPidPhysicsConstants::pid("e+"));
	filler->AddTrack(HPidPhysicsConstants::pid("e-"));
	filler->AddTrack(HPidPhysicsConstants::pid("e+"));
	filler->AddTrack(HPidPhysicsConstants::pid("e-"));
	filler->AddTrack(HPidPhysicsConstants::pid("pi+"));
	filler->AddTrack(HPidPhysicsConstants::pid("pi-"));
	filler->AddTrack(HPidPhysicsConstants::pid("p"));
	filler->AddTrack(HPidPhysicsConstants::pid("p"));

	filler->just_a_test_for_debugging();
}