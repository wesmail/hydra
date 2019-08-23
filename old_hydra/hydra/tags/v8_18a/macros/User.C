{
 class User {
  User(void) {}
  ~User(void) {}
  void execute(HEvent *event) {
   printf("Number: %i",event->getHeader()->getSeqNumber());
  }
 };
}
