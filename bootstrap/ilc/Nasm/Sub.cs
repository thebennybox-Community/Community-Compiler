namespace ilc.Nasm
{
    public class Sub
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Sub(Registers destination, Registers source)
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