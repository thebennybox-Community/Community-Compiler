namespace ilc.Nasm
{
    public class Shl
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Shl(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"shl {Destination}, {Source}";
        }
    }
}