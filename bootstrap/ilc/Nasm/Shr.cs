namespace ilc.Nasm
{
    public class Shr
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Shr(Registers destination, Registers source)
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