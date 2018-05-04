namespace ilc.Nasm
{
    public class Sub
    {
        public Registers Destination { get; set; }
        public object Source { get; set; }

        public Sub(Registers destination, object source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"sub {Destination}, {Source}";
        }
    }
}