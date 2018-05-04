using System;

namespace ilc.Nasm
{
    public class Mov
    {
        public Registers Destination { get; set; }
        public object Source { get; set; }

        public int DestinationOffset { get; set; }
        public int SourceOffset { get; set; }
        
        public bool DestinationDeRef { get; set; }
        public bool SourceOffsetDeRef { get; set; }

        public Mov(Registers destination, object source)
        {
            Destination = destination;
            Source = source;
        }

        public override string ToString()
        {
            if (DestinationOffset != 0 || DestinationDeRef)
            {
                return $"mov [{Destination}{(DestinationOffset < 0 ? "" : "+") + DestinationOffset}], {Source}";
            }

            if (SourceOffset != 0 || SourceOffsetDeRef)
            {
                return $"mov {Destination}, [{Source}{(SourceOffset < 0 ? "" : "+") + SourceOffset}]";
            }

            return $"mov {Destination}, {Source}";
        }
    }
}