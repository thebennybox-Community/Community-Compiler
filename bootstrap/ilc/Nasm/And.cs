namespace ilc.Nasm
{
    public class And
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public And(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"and {Destination}, {Source}";
        }
    }
}