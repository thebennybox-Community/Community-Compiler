namespace ilc.Nasm
{
    public class Xor
    {
        public Registers Destination { get; set; }
        public Registers Source { get; set; }

        public Xor(Registers destination, Registers source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            return $"xor {Destination}, {Source}";
        }
    }
}