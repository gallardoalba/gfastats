//
//  gfastats-classes.h
//  
//
//  Created by Giulio Formenti on 12/30/21.
//

#ifndef gfastatscommons_h
#define gfastatscommons_h

//classes
class BedCoordinates { // generic representation of bed coordinates
private:
    std::vector<std::string> seqHeaders;
    std::vector<unsigned int> cBegin;
    std::vector<unsigned int> cEnd;
    
public:
    
    void pushCoordinates(std::string h, unsigned int b = 0, unsigned int e = 0) { // reading coordinates
        
        seqHeaders.push_back(h);
        cBegin.push_back(b);
        cEnd.push_back(e);
        
    }
    
    bool empty() {
        
        return (seqHeaders.size()==0) ? true : false; // check if no coordinates are present
        
    }
    
    std::vector<std::string> getSeqHeaders() { // get all the headers
        
        return seqHeaders;
        
    }
    
    std::string getSeqHeader(unsigned int pos) { // get a specific header
        
        return seqHeaders[pos];
        
    }
    
    unsigned int getcBegin(unsigned int pos) { // get a specific start coordinate
        
        return cBegin[pos];
        
    }
    
    unsigned int getcEnd(unsigned int pos) { // get a specific end coordinate
        
        return cEnd[pos];
        
    }
    
};

class InSegment { // DNA sequence with no gaps
private:
    std::string seqHeader;
    std::string seqComment;
    std::string inSequence;
    std::string inSequenceQuality;
    unsigned int A = 0, C = 0, G = 0, T = 0, lowerCount = 0;
    
    friend class SAK;
    friend class InSequences;
    
public:
    
    void setSeqHeader(std::string* h) {
        seqHeader = *h;
    }
    
    void setSeqComment(std::string c) {
        seqComment = c;
    }
    
    void setInSequence(std::string* s) {
        inSequence = *s;
    }
    
    void setInSequenceQuality(std::string* q) {
        inSequenceQuality = *q;
    }
    
    std::string getSeqHeader() {
        return seqHeader;
    }
    
    std::string getSeqComment() {
        return seqComment;
    }
    
    std::string getInSequence() {
        return inSequence;
    }
    
    std::string getInSequenceQuality() {
        
        return inSequenceQuality;
    }
    
    unsigned int getSegmentLen() {
        return inSequence.size();
    }
        
    void setACGT(unsigned int* a, unsigned int* c, unsigned int* g, unsigned int* t) {
        
        A = *a;
        C = *c;
        G = *g;
        T = *t;
        
    }
    
    void setLowerCount(unsigned int* C) {
        
        lowerCount = *C;
        
    }
    
    unsigned int getA() {
        
        return A;
    }
    
    unsigned int getC() {
        
        return C;
    }
    
    unsigned int getG() {
        
        return G;
    }
    
    unsigned int getT() {
        
        return T;
    }

    unsigned int getLowerCount() {
        
        return lowerCount;
    }

    unsigned int getSegmentLength() {
        
        return inSequence.size();
    }
    
    double computeGCcontent() {
        
        double GCcontent = (double) (G + C) / (G + C + A + T) * 100;
        
        return GCcontent;
    }
    
};

class InGap {
private:
    unsigned long long int lineN; // useful if we wish to sort as is the original input
    std::string gId, sId1, sId2;
    char sId1Or, sId2Or;
    unsigned int dist;
    int counter = 0;
    
    friend class SAK;
    friend class InSequences;
    
public:
    void newGap(const std::string& gid, const std::string& sid1, const std::string& sid2, const char& sid1or, const char& sid2or, unsigned int& d) {
        
        gId = gid;
        sId1 = sid1;
        sId2 = sid2;
        sId1Or = sid1or;
        sId2Or = sid2or;
        dist = d;
        
    }
    
    bool readLine(std::string* newLine, unsigned long long int* lN) {
    
        lineN = *lN;
        
        gId = strtok(strdup((*newLine).c_str()),"\t"); // strip G
        
        gId = strtok(NULL,"\t");
        
        sId1 = strtok(NULL,"\t");
        sId2 = strtok(NULL,"\t");
        
        sId1Or = sId1.back(); // get sequence orientation in the gap
        sId2Or = sId2.back();
        
        sId1.pop_back(); // remove sequence orientation in the gap
        sId2.pop_back();
        
        dist = std::stoi(strtok(NULL,"\t"));
        
        counter++;
        
        return true;
        
    }
    
    std::string getgId() {
        
        return gId;
        
    }
    
    std::string getsId1() {
        
        return sId1;
        
    }
    
    char getsId1Or() {
        
        return sId1Or;
        
    }
    
    std::string getsId2() {
        
        return sId2;
        
    }
    
    char getsId2Or() {
        
        return sId2Or;
        
    }
    
    unsigned int getDist() {
        
        return dist;
        
    }
    
};
class InEdges {};
class InOlines {};
class InUlines {};

class InSequences { //collection of InSegment and inGap objects and their summary statistics
    
private:
    std::vector<InSegment> inSegments = std::vector<InSegment>();
    
    //gfa variables
    std::vector<InGap> inGaps;
    std::vector<std::vector<Tuple>> adjListFW;
    std::vector<std::vector<Tuple>> adjListBW;
    std::unordered_map<std::string, unsigned long long int> ump;
    std::unordered_map<int, bool> visited;
    bool backward = false;
    //InEdges inEdges;
    //InOlines inOlines;
    //InUlines inUlines;
    
    std::vector<unsigned int> scaffLens;
    std::vector<unsigned int> contigLens;
    std::vector<unsigned int> gapLens;
    
    std::vector<unsigned int> scaffNstars   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> scaffLstars   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> scaffNGstars  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> scaffLGstars  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::vector<unsigned int> contigNstars  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> contigLstars  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> contigNGstars {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> contigLGstars {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::vector<unsigned int> gapNstars     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<unsigned int> gapLstars     {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    double scaffAuN = 0, scaffAuNG = 0, contigAuN = 0, contigAuNG = 0, gapAuN = 0;
    
    InSegment inSegment;
    InGap gap;
    
    unsigned long long int totScaffLen = 0;
    unsigned long long int totSegmentLen = 0;
    
    unsigned int
    scaffN = 0,
    totGapLen = 0,
    segUniqN = 0, // unique numeric identifier for each segment
    gapUniqN = 0; // unique numeric identifier for each gap
    
    unsigned long int totA = 0;
    unsigned long int totC = 0;
    unsigned long int totG = 0;
    unsigned long int totT = 0;
    unsigned long int totLowerCount = 0;
    
    friend class SAK;
    
public:
    
    void addSegment(std::string* seqHeader, std::string* seqComment, std::string* sequence, unsigned int* A, unsigned int* C, unsigned int* G, unsigned int* T, unsigned int* lowerCount, std::string* sequenceQuality = NULL) {
        
        // operation on the segment
        
        inSegment.setSeqHeader(seqHeader);
        
        if (seqComment != NULL) {
            
            inSegment.setSeqComment(*seqComment);
            
        }
        
        verbose(verbose_flag, "Processing segment: " + *seqHeader);
        
        inSegment.setInSequence(sequence);
        
        verbose(verbose_flag, "Sequence set");
        
        if (sequenceQuality != NULL) {
            
            inSegment.setInSequenceQuality(sequenceQuality);
            
            verbose(verbose_flag, "Sequence quality set");
            
        }
        
        inSegment.setACGT(A, C, G, T);
        
        verbose(verbose_flag, "Increased ACGT counts");
        
        inSegment.setLowerCount(lowerCount);

        verbose(verbose_flag, "Increased count of lower bases");
        
        inSegments.push_back(inSegment); // adding segment to segment set
        
        // operations of the segment set
        
        verbose(verbose_flag, "Segment added to sequence vector");
        
        unsigned int seqSize = sequence->size();
    
        contigLens.push_back(seqSize);
        
        verbose(verbose_flag, "Recorded length of sequence");
        
        increaseTotSegmentLen(seqSize);
        
        verbose(verbose_flag, "Increased total segment length");
        
    }
    
    void traverseInSequence(std::string* seqHeader, std::string* seqComment, std::string* sequence, std::string* sequenceQuality = NULL) { // traverse the sequence to split at gaps and measure sequence properties
        
        unsigned int pos = 0, // current position in sequence
                     A = 0, C = 0, G = 0, T = 0,
                     lowerCount = 0,
                     dist = 0, // gap size
                     n = 2, // dynamic value for segment ids depending on gap type
                     segN = 1, gapN = 1, // segment and gap numeric identifiers
                     sStart = 0, sEnd = 0; // segment start and end
        char sign = '+';
        bool wasN = false, pushbackSegment = false, pushbackGap = false;

        std::string sequenceSubSeq, sequenceQualitySubSeq, newSeqHeader;
        
        unsigned int seqLen = sequence->length()-1;
 
        for (char &base : *sequence) {

            if (islower(base)) {

                lowerCount++;

            }

            switch (base) {

                case 'N':
                case 'n':
                case 'X':
                case 'x': {
                    
                    dist++;
                    
                    if (!wasN) { // gap start
                        
                        if (pos>0) {  // gap not at the start of the sequence
                            
                            pushbackSegment = true;
                            sEnd = pos - 1;
                        
                        }
                        
                        pushbackGap = false;

                    }

                    if(pos == seqLen) { // end of scaffold
                        
                        sign = '-';
                        
                        if (!wasN){ // case of a single bp gap at the end
                            
                            pushbackSegment = true;
                            sEnd = pos - 1;

                        }else{
                         
                            pushbackSegment = false;
                            
                        }
                        n = 1;
                        pushbackGap = true;

                    }

                    wasN = true;

                    break;
                }
                default: {

                    if (wasN) { // internal gap end
                        
                        pushbackGap = true;
                        sStart = pos;
                        
                        pushbackSegment = false;

                    }
                    
                    if (pos == seqLen) {
                        
                        pushbackSegment = true;
                        sEnd = pos;
                        
                        pushbackGap = false;
                        
                    }
                    
                    if (wasN && pos == seqLen) { // single base at the end of sequence
                        
                        pushbackGap = true;
                        
                        pushbackSegment = true;

                    }

                    switch (base) {
                        case 'A':
                        case 'a':{

                            A++;
                            break;

                        }
                        case 'C':
                        case 'c':{

                            C++;
                            break;

                        }
                        case 'G':
                        case 'g': {

                            G++;
                            break;

                        }
                        case 'T':
                        case 't': {

                            T++;
                            break;

                        }

                    }

                    wasN = false;

                }

            }
            
            if (pushbackSegment) {
                
                newSeqHeader = *seqHeader + "." + std::to_string(segN);
                
                insertHash(newSeqHeader, segUniqN);
                
                sequenceSubSeq = sequence->substr(sStart, sEnd + 1 - sStart);
                
                if (sequenceQuality != NULL) {
                
                    sequenceQualitySubSeq = sequenceQuality->substr(sStart, sEnd + 1 - sStart);
                
                }
                
                addSegment(&newSeqHeader, seqComment, &sequenceSubSeq, &A, &C, &G, &T, &lowerCount, &sequenceQualitySubSeq);
                pushbackSegment = false;
                
                segN++;
                segUniqN++;
                

            }
            if (pushbackGap) {
                
                if (pos - dist == 0) { // gap at the start
                    
                    segN++; // to prevent overflow
                    
                }
            
                gap.newGap(*seqHeader + "." + std::to_string(gapN), *seqHeader + "." + std::to_string((pos == seqLen) ? segN-n : segN-1), *seqHeader + "." + std::to_string((pos == seqLen || pos - dist == 0) ? segN-1 : segN), '+', sign, dist);
                
                appendGFAGap(gap);

                pushbackGap = false;
                
                if (pos - dist == 0) { // gap at the start
                    
                    segN--; // to restore original count
                    
                }
                
                dist=0;
                gapN++; // number of gaps in the current scaffold
                gapUniqN++; // gap unique numeric identified

            }

            pos++;

        }
        
        increaseTotScaffLen(seqLen+1);

        verbose(verbose_flag, "Increased total scaffold length");

        recordScaffLen(seqLen+1);

        verbose(verbose_flag, "Recorded length of sequence");
        
    }
    
    void appendSequence(std::string* seqHeader, std::string* seqComment, std::string* sequence, std::string* sequenceQuality = NULL) { // method to append a new sequence
        
        verbose(verbose_flag, "Header, comment, sequence and (optionally) quality read");
        
        traverseInSequence(seqHeader, seqComment, sequence, sequenceQuality);
        
        verbose(verbose_flag, "Sequence traversed");
        
        scaffN++;
        
        verbose(verbose_flag, "Increased total scaffold N");
        
        if(verbose_flag) {std::cout<<"\n";};
        
    }
    
    bool updateStats() {
        
        for (InSegment inSegment : inSegments) {
            
            totA += inSegment.A;
            totC += inSegment.C;
            totG += inSegment.G;
            totT += inSegment.T;

            increaseTotLowerCount(inSegment.lowerCount);
            
        }
        
        verbose(verbose_flag, "Increased ACGT counts");
        verbose(verbose_flag, "Increased count of lower bases");
        
        return true;
        
    }
    
    unsigned int getSegmentN() {
        
        return inSegments.size();
    }

    InSegment getInSegment(unsigned int &idx) {
        
        InSegment inSegment = inSegments[idx];
        return inSegment;
        
    }
    
    std::vector<InSegment> getInSegments() {
        
        return inSegments;
        
    }
    
    std::vector<InGap> getInGaps() {
        
        return inGaps;
        
    }
    
    void increaseTotScaffLen(unsigned int ScaffLen) {
        
        totScaffLen += ScaffLen;
        
    }
    
    unsigned long long int getTotScaffLen() {
        
        return totSegmentLen+totGapLen;
        
    }
    
    void increaseTotSegmentLen(unsigned int segmentLen) {
        
        totSegmentLen += segmentLen;
        
    }
    
    unsigned long long int getTotSegmentLen() {
        
        return totSegmentLen;
        
    }
    
    void increaseTotGapLen(unsigned int gapLen) {
        
        totGapLen += gapLen;
        
    }
    
    unsigned int getTotGapLen() {
        
        return totGapLen;
        
    }
    
    unsigned int getGapN() {
        
        return inGaps.size();
        
    }
    
    void recordScaffLen(unsigned int seqLen) {
        
        scaffLens.push_back(seqLen);
        
    }
    
    void recordGapLen(unsigned int gapLen) {
        
        gapLens.push_back(gapLen);
        
    }
    
    void evalNstars(char type, unsigned long long int gSize = 0) { // switch between scaffold, contig, gap while computing N* statistics
        
        switch(type) {
                
            case 's': {
                
                computeNstars(scaffLens, scaffNstars, scaffLstars, &scaffNGstars, &scaffLGstars, gSize);
                break;
                
            }
                
            case 'c': {
                
                computeNstars(contigLens, contigNstars, contigLstars, &contigNGstars, &contigLGstars, gSize);
                break;
                
            }
                
            case 'g': {
                
                computeNstars(gapLens, gapNstars, gapLstars);
                break;
                
            }
                
        }
        
    }
    
    
    void computeNstars(std::vector<unsigned int>& lens, // compute N/L* statistics, vector of all lengths
                       std::vector<unsigned int>& Nstars,      std::vector<unsigned int>& Lstars, // required arguments are passed by reference
                       std::vector<unsigned int>* NGstars = 0, std::vector<unsigned int>* LGstars = 0, unsigned long long int gSize = 0) { // optional arguments are passed by pointer
        
        sort(lens.begin(), lens.end(), std::greater<unsigned int>()); // sort lengths Z-A
        
        unsigned long long int sum = 0, totLen = 0;
        
        for(std::vector<unsigned int>::iterator it = lens.begin(); it != lens.end(); ++it) // find total length
            totLen += *it;
        
        short int N = 1, NG = 1;
        
        for(unsigned int i = 0; i < lens.size(); i++) { // for each length
            
            sum += lens[i]; // increase sum
            
            while (sum >= ((double) totLen / 10 * N) && N<= 10) { // conditionally add length.at or pos to each N/L* bin
                
                Nstars[N-1] = lens[i];
                Lstars[N-1] = i + 1;
                
                N = N + 1;
                
            }
                
            while (gSize > 0 && (sum >= ((double) gSize / 10 * NG)) && NG<= 10) { // if not computing gap statistics repeat also for NG/LG* statistics
                
                (*NGstars)[NG-1] = lens[i];
                (*LGstars)[NG-1] = i + 1;
                
                NG = NG + 1;
                
            }
            
        }
        
    }
    
    void evalAuN(char type, unsigned long long int gSize = 0) { // switch between scaffold, contig, gap while computing N* statistics
        
        switch(type) {
                
            case 's': {
                
                computeAuN(scaffLens, scaffAuN, &scaffAuNG, gSize);
                break;
                
            }
                
            case 'c': {
                
                computeAuN(contigLens, contigAuN, &contigAuNG, gSize);
                break;
                
            }
                
            case 'g': {
                
                computeAuN(gapLens, gapAuN);
                break;
                
            }
                
        }
        
    }
    
    void computeAuN(std::vector<unsigned int>& lens, double& auN, double* auNG = 0, unsigned long long int gSize = 0) {// compute N* statistics
        
        unsigned long long int totLen = 0;
        
        for(std::vector<unsigned int>::iterator it = lens.begin(); it != lens.end(); ++it) // find total length
            totLen += *it;
        
        for(unsigned int i = 0; i < lens.size(); i++) { // for each length
            
            auN += (double) lens[i] * lens[i] / totLen; // the area under the curve is the length (height) * fraction of the total length
            
            if (gSize > 0) {
                
                *auNG += (double) lens[i] * lens[i] / gSize;
                
            }
            
        }
        
    }
    
    unsigned int getScaffN() {
        
        return scaffN;
        
    }
    
    std::vector <unsigned int> getScaffNstars() {
        
        return scaffNstars;
        
    }
    
    std::vector <unsigned int> getScaffNGstars() {
        
        return scaffNGstars;
        
    }
    
    std::vector <unsigned int> getScaffLstars() {
        
        return scaffLstars;
        
    }
    
    std::vector <unsigned int> getScaffLGstars() {
        
        return scaffLGstars;
        
    }
    
    std::vector <unsigned int> getContigNstars() {
        
        return contigNstars;
        
    }
    
    std::vector <unsigned int> getContigNGstars() {
        
        return contigNGstars;
        
    }
    
    std::vector <unsigned int> getContigLstars() {
        
        return contigLstars;
        
    }
    
    std::vector <unsigned int> getContigLGstars() {
        
        return contigLGstars;
        
    }
    
    std::vector <unsigned int> getGapNstars() {
        
        return gapNstars;
        
    }
    
    std::vector <unsigned int> getGapLstars() {
        
        return gapLstars;
        
    }
    
    unsigned int getScaffN50() {
        
        return scaffNstars[4];
        
    }
    
    unsigned int getScaffNG50() {
        
        return scaffNGstars[4];
        
    }
    
    unsigned int getScaffL50() {
        
        return scaffLstars[4];
        
    }
    
    unsigned int getScaffLG50() {
        
        return scaffLGstars[4];
        
    }
    
    double getScaffauN() {
        
        return scaffAuN;
        
    }
    
    double getScaffauNG() {
        
        return scaffAuNG;
        
    }
    
    double getContigauN() {
        
        return contigAuN;
        
    }
    
    double getContigauNG() {
        
        return contigAuNG;
        
    }
    
    double getGapauN() {
        
        return gapAuN;
        
    }
    
    unsigned int getContigNs() {
        
        return inSegments.size();
        
    }
    
    unsigned int getContigN50() {
        
        return contigNstars[4]; // middle value
        
    }
    
    unsigned int getContigNG50() {
        
        return contigNGstars[4];
        
    }
    
    unsigned int getContigL50() {
        
        return contigLstars[4];
        
    }
    
    unsigned int getContigLG50() {
        
        return contigLGstars[4];
        
    }
    
    unsigned int getGapN50() {
        
        return gapNstars[4];
        
    }
    
    unsigned int getGapL50() {
        
        return gapLstars[4];
        
    }
    
    unsigned int getLargestScaffold() {
            
        return scaffLens[0]; // sorted during N/L* computation
        
    }
    
    unsigned int getLargestContig() {
            
        return contigLens[0]; // sorted during N/L* computation
        
    }
    
    unsigned int getLargestGap() {
        
        return gapLens.size() > 0 ? gapLens[0] : 0; // sorted during N/L* computation
        
    }
    
    double computeAverageScaffLen() {
        
        return (double) totScaffLen/scaffN;
        
    }
    
    double computeAverageSegmentLen() {
        
        return (double) totSegmentLen/contigLens.size();
        
    }
    
    double computeAverageGapLen() {
        
        return (double) totGapLen/gapLens.size();
        
    }
    
    void increaseTotLowerCount(unsigned int C) {
        
        totLowerCount += C;
        
    }
    
    unsigned long int getTotA() {
        
        return totA;
    }
    
    unsigned long int getTotC() {
        
        return totC;
    }
    
    unsigned long int getTotG() {
        
        return totG;
    }
    
    unsigned long int getTotT() {
        
        return totT;
    }
    
    unsigned long int getTotLowerCount() {
        
        return totLowerCount;
    }
    
    double computeGCcontent() {
        
        double GCcontent;
        
        if (inSegments.size()>0) {
            
            GCcontent = (double) (totC + totG) / (totA + totC + totG + totT) * 100;
            
        }else{
            
            GCcontent = 0;
            
        }
        
        return GCcontent;
    }
    
    //gfa methods
    bool appendGFAGap(InGap inGap) {
        
        recordGapLen(inGap.dist);
        
        verbose(verbose_flag, "Recorded length of gaps in sequence");
        
        increaseTotGapLen(inGap.dist);
        
        verbose(verbose_flag, "Increased total gap length");
        
        inGaps.push_back(inGap);
        
        return true;
        
    }
    
    std::vector<InGap> getGFAGaps() { // return gfa gaps vector
        
        return inGaps;
        
    }
    
    //gfa methods
    void insertHash(std::string segHeader, unsigned long long int segUniqN) {
    
        ump.insert({segHeader, segUniqN});
    
    }
        
    void buildGraph(std::vector<InGap> const& edges) // graph Constructor
    {
        
        verbose(verbose_flag, "Building graph");
        
        adjListFW.resize(inSegments.size()); // resize the vertex vector to hold `n` elements
        adjListBW.resize(inSegments.size()); // resize the vertex vector to hold `n` elements
 
        
        for (auto &edge: edges) // add edges to the graph
        {
            
            adjListFW.at(ump.at(edge.sId1)).push_back(std::make_tuple(edge.sId1Or, ump.at(edge.sId2), edge.sId2Or, edge.dist)); // insert at gap start gap destination and weight (gap size)
            adjListBW.at(ump.at(edge.sId2)).push_back(std::make_tuple(edge.sId2Or, ump.at(edge.sId1), edge.sId1Or, edge.dist)); // undirected graph
            
        }
        
        verbose(verbose_flag, "Graph built");
        
    }
    
    void dfsSeq(int v, std::string &inSequence, std::string* inSequenceQuality = NULL) // Depth First Search
    {
    
        visited[v] = true; // mark the current node as visited
        std::string inSequenceNext;
        std::string inSequenceQualityNext;
         
         if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 1 && !(std::get<1>(adjListFW.at(v).at(0)) == std::get<1>(adjListBW.at(v).at(0))) && !backward) { // if the vertex has exactly one forward and one backward connection and they do not connect to the same vertex (internal node)
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case a: internal node, forward direction");
             
             inSequence = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSequence : revCom(inSequence); // check if vertex should be in forward orientation, if not reverse-complement
             
             inSequenceNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence()); // check if vertex should be in forward orientation, if not reverse-complement
             
             inSequence += inSequenceNext;
             
             if (!(inSequenceQuality == NULL)) {
                 
                 *inSequenceQuality = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? *inSequenceQuality : rev(*inSequenceQuality); // check if vertex should be in forward orientation, if not reverse-complement
             
                 inSequenceQualityNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality()); // check if vertex should be in forward orientation, if not reverse-complement
                 
                 *inSequenceQuality += inSequenceQualityNext;
                 
             }
             
             backward = false;
                 
         }else if (adjListFW.at(v).size() == 0 && adjListBW.at(v).size() == 1){ // this is the final vertex without gaps
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case b: end node, forward direction, no final gap");
             
             inSequenceNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());
             
             inSequence += inSequenceNext;
             
             if (!(inSequenceQuality == NULL)) {
             
                 inSequenceQualityNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality());
             
                 *inSequenceQuality += inSequenceQualityNext;
                 
             }
             
             backward = true; // reached the end
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 2){ // this is the final vertex with terminal gap
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case c: end node, forward direction, final gap");
             
             inSequenceNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());
             
             inSequence += inSequenceNext;
             
             inSequence += std::string(std::get<3>(adjListFW.at(v).at(0)), 'N'); // add gap
             
             if (!(inSequenceQuality == NULL)) {
             
                 inSequenceQualityNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality());
             
                 *inSequenceQuality += inSequenceQualityNext;
             
                 *inSequenceQuality += std::string(std::get<3>(adjListFW.at(v).at(0)), '!'); // add missing quality
                 
             }
             
             backward = true; // reached the end
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 1 && !(std::get<1>(adjListFW.at(v).at(0)) == std::get<1>(adjListBW.at(v).at(0))) && backward){ // this is an intermediate vertex, only walking back
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case d: intermediate node, backward direction");
             
             inSequenceNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());

             inSequence.insert(0, inSequenceNext);
             
             if (!(inSequenceQuality == NULL)) {
             
                 inSequenceQualityNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality());
             
                 (*inSequenceQuality).insert(0, inSequenceQualityNext);
             
             }
             
             backward = true;
            
         }else if(adjListFW.at(v).size() == 0 && adjListBW.at(v).size() == 0){ // disconnected component
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case e: disconnected component");
             
             inSequence += inSegments[v].getInSequence();
             
             if (!(inSequenceQuality == NULL)) {
             
                 *inSequenceQuality += inSegments[v].getInSequenceQuality();
                 
             }
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 0){ // this is the first vertex without gaps
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case f: start node, no gaps");
                 
             inSequenceNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());
             
             inSequence.insert(0, inSequenceNext);
             
             if (!(inSequenceQuality == NULL)) {
             
                 inSequenceQualityNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality());
             
                 (*inSequenceQuality).insert(0, inSequenceQualityNext);
                 
             }
             
             backward = false;
             
         }else if (adjListFW.at(v).size() == 2 && adjListBW.at(v).size() == 1){ // this is the first vertex with a terminal gap
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case g: start node, start gap");
             
             inSequenceNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());
             
             inSequence.insert(0, inSequenceNext);
             
             inSequence.insert(0, std::string(std::get<3>(adjListBW.at(v).at(0)), 'N')); // add gap
             
             if (!(inSequenceQuality == NULL)) {
             
                 (*inSequenceQuality).insert(0, std::string(std::get<3>(adjListBW.at(v).at(0)), '!')); // add missing quality
             
                 inSequenceQualityNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality());
             
                 (*inSequenceQuality).insert(0, inSequenceQualityNext);
                 
             }
             
             backward = false;
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 1 && std::get<1>(adjListFW.at(v).at(0)) == std::get<1>(adjListBW.at(v).at(0))) { // if the vertex has exactly one forward and one backward connection and they connect to the same vertex (disconnected component with gap)
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case h: disconnected component with gap");
             
             inSequenceNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());
             
             inSequence += inSequenceNext;
             
             std::get<2>(adjListFW.at(v).at(0)) == '-' ? inSequence += std::string(std::get<3>(adjListFW.at(v).at(0)), 'N') : inSequence.insert(0,std::string(std::get<3>(adjListBW.at(v).at(0)), 'N')); // add gap: if - in second vertex is terminal else is start gap
             
             if (!(inSequenceQuality == NULL)) {
             
                 inSequenceQualityNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequenceQuality() : rev(inSegments[v].getInSequenceQuality());
             
                 *inSequenceQuality += inSequenceQualityNext;
             
                 std::get<2>(adjListFW.at(v).at(0)) == '-' ? *inSequenceQuality += std::string(std::get<3>(adjListFW.at(v).at(0)), '!') : (*inSequenceQuality).insert(0,std::string(std::get<3>(adjListBW.at(v).at(0)), '!')); // add missing sequence: if - in second vertex is terminal else is start gap
                 
             }
             
             backward = false;
             
         }
         
         for (Tuple i: adjListFW[v]) { // recur for all forward vertices adjacent to this vertex
             
             if (!visited[std::get<1>(i)]) {
                 
                 inSequence += std::string(std::get<3>(i), 'N'); // add gaps
                 
                 if (!(inSequenceQuality == NULL)) {
                 
                     *inSequenceQuality += std::string(std::get<3>(i), '!'); // add missing quality
                     
                 }
                 
                 dfsSeq(std::get<1>(i), inSequence, inSequenceQuality); // recurse
                 
             }
         }
         
         for (Tuple i: adjListBW[v]) { // recur for all backward vertices adjacent to this vertex
             
             if (!visited[std::get<1>(i)]) {

                 inSequence.insert(0, std::string(std::get<3>(i), 'N')); // add gaps
                 
                 if (!(inSequenceQuality == NULL)) {
                 
                     (*inSequenceQuality).insert(0, std::string(std::get<3>(i), '!')); // add missing quality
                     
                 }

                 dfsSeq(std::get<1>(i), inSequence, inSequenceQuality); // recurse

             }
         }
        
    }
    
    void dfsAgp(int v, std::string &agp, unsigned int &cStart, unsigned int &cEnd) // Depth First Search
    {
    
        visited[v] = true; // mark the current node as visited
        std::string agpNext, seqHeader;
        
        seqHeader = inSegments[v].getSeqHeader();
        seqHeader.pop_back();
        seqHeader.pop_back();
         
         if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 1 && !(std::get<1>(adjListFW.at(v).at(0)) == std::get<1>(adjListBW.at(v).at(0))) && !backward) { // if the vertex has exactly one forward and one backward connection and they do not connect to the same vertex (internal node)
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case a: internal node, forward direction");
             
             cEnd = cStart + inSegments[v].getInSequence().size() - 1;
             
             agpNext = seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+inSegments[v].getSeqHeader().substr(inSegments[v].getSeqHeader().length() - 1)+"\tW\t"+inSegments[v].getSeqHeader()+"\t1\t"+std::to_string(inSegments[v].getInSequence().size())+"\t+\n";
             
             agp += agpNext;
             
             cStart = cEnd + 1;
             
             backward = false;
                 
         }else if (adjListFW.at(v).size() == 0 && adjListBW.at(v).size() == 1){ // this is the final vertex without gaps
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case b: end node, forward direction, no final gap");
             
             cEnd = cStart + inSegments[v].getInSequence().size() - 1;
             
             agpNext = seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+inSegments[v].getSeqHeader().substr(inSegments[v].getSeqHeader().length() - 1)+"\tW\t"+inSegments[v].getSeqHeader()+"\t1\t"+std::to_string(inSegments[v].getInSequence().size())+"\t+\n";
             
             agp += agpNext;
             
             cStart = cEnd + 1;
             
             backward = true; // reached the end
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 2){ // this is the final vertex with terminal gap
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case c: end node, forward direction, final gap");
             
             cEnd = cStart + inSegments[v].getInSequence().size() - 1;
             
             agpNext = seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+inSegments[v].getSeqHeader().substr(inSegments[v].getSeqHeader().length() - 1)+"\tW\t"+inSegments[v].getSeqHeader()+"\t1\t"+std::to_string(inSegments[v].getInSequence().size())+"\t+\n";
             
             agp += agpNext;
             
             cStart = cEnd + 1;
             
             cEnd = cStart + std::get<3>(adjListFW.at(v).at(0)) - 1;
             
             agp += seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+std::to_string(std::get<3>(adjListFW.at(v).at(0)))+"\tN\t"+std::to_string(std::get<3>(adjListFW.at(v).at(0)))+"\tscaffold\tyes\n"; // add gaps
             
             cStart = cEnd + 1;
             
             backward = true; // reached the end
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 1 && !(std::get<1>(adjListFW.at(v).at(0)) == std::get<1>(adjListBW.at(v).at(0))) && backward){ // this is an intermediate vertex, only walking back
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case d: intermediate node, backward direction");
             
             agpNext = (std::get<0>(adjListBW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());

             agp.insert(0, agpNext);
             
             backward = true;
            
         }else if(adjListFW.at(v).size() == 0 && adjListBW.at(v).size() == 0){ // disconnected component
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case e: disconnected component");
             
             agp += inSegments[v].getInSequence();
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 0){ // this is the first vertex without gaps
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case f: start node, no gaps");
                 
             cEnd = cStart + inSegments[v].getInSequence().size() - 1;
             
             agpNext = seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+inSegments[v].getSeqHeader().substr(inSegments[v].getSeqHeader().length() - 1)+"\tW\t"+inSegments[v].getSeqHeader()+"\t1\t"+std::to_string(inSegments[v].getInSequence().size())+"\t"+std::get<0>(adjListFW.at(v).at(0))+"\n";
             
             agp += agpNext;
             
             cStart = cEnd + 1;
             
             backward = false;
             
         }else if (adjListFW.at(v).size() == 2 && adjListBW.at(v).size() == 1){ // this is the first vertex with a terminal gap
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case g: start node, start gap");
             
             agpNext = (std::get<0>(adjListFW.at(v).at(0)) == '+') ? inSegments[v].getInSequence() : revCom(inSegments[v].getInSequence());
             
             agp.insert(0, agpNext);
             
             agp.insert(0, std::string(std::get<3>(adjListBW.at(v).at(0)), 'N')); // add gap
             
             backward = false;
             
         }else if (adjListFW.at(v).size() == 1 && adjListBW.at(v).size() == 1 && std::get<1>(adjListFW.at(v).at(0)) == std::get<1>(adjListBW.at(v).at(0))) { // if the vertex has exactly one forward and one backward connection and they connect to the same vertex (disconnected component with gap)
             
             verbose(verbose_flag, "node: " + std::to_string(v) + " --> case h: disconnected component with gap");
             
             std::get<2>(adjListFW.at(v).at(0)) == '-' ? // if negative gap (-)
             cStart = 1 :
             cStart = std::get<3>(adjListFW.at(v).at(0)) + 1;
             
             cEnd = cStart + inSegments[v].getInSequence().size() - 1;
             
             agpNext = seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+inSegments[v].getSeqHeader().substr(inSegments[v].getSeqHeader().length() - 1)+"\tW\t"+inSegments[v].getSeqHeader()+"\t1\t"+std::to_string(inSegments[v].getInSequence().size())+"\t+\n";
             
             agp += agpNext;
             
             cStart = cEnd + 1;
             
             cEnd = cStart + std::get<3>(adjListFW.at(v).at(0)) - 1;
             
             std::get<2>(adjListFW.at(v).at(0)) == '-' ? // if negative gap (-)
             agp += seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t1\tN\t"+std::to_string(std::get<3>(adjListFW.at(v).at(0)))+"\tscaffold\tyes\n": // insert at the end
             agp.insert(0,seqHeader+"\t1\t"+std::to_string(std::get<3>(adjListFW.at(v).at(0)))+"\t1\tN\t"+std::to_string(std::get<3>(adjListFW.at(v).at(0)))+"\tscaffold\tyes\n"); // insert at the beginning

             
             backward = false;
             
         }
         
         for (Tuple i: adjListFW[v]) { // recur for all forward vertices adjacent to this vertex
             
             if (!visited[std::get<1>(i)]) {
                 
                 cEnd = cStart + std::get<3>(i) - 1;
                 
                 agp += seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+std::to_string(std::get<3>(i))+"\tN\t"+std::to_string(std::get<3>(i))+"\tscaffold\tyes\n"; // add gaps
                 
                 cStart = cEnd + 1;
                 
                 dfsAgp(std::get<1>(i), agp, cStart, cEnd); // recurse
                 
             }
         }
         
         for (Tuple i: adjListBW[v]) { // recur for all backward vertices adjacent to this vertex
             
             if (!visited[std::get<1>(i)]) {
                 
                 cEnd = cEnd + std::get<3>(i) - 1;

                 agp += seqHeader+"\t"+std::to_string(cStart)+"\t"+std::to_string(cEnd)+"\t"+std::to_string(std::get<3>(i))+"\tN\t"+std::to_string(std::get<3>(i))+"\tscaffold\tyes\n"; // add gaps
                 
                 cStart = cEnd + 1;

                 dfsAgp(std::get<1>(i), agp, cStart, cEnd); // recurse

             }
         }
        
    }
    
    std::vector<std::vector<Tuple>> getAdjListFW() {
    
        return adjListFW;
        
    }
    
    std::vector<std::vector<Tuple>> getAdjListBW() {
    
        return adjListBW;
        
    }
    
    bool getVisited(unsigned long long int segUniqN) {
    
        return visited[segUniqN];
        
    }
        
    //end of gfa methods
    
};

class SAK { // the swiss army knife
private:
    InSequences inSequences;
    InSegment inSegment1, inSegment2, inSegmentNew;
    std::string sId1Header, sId2Header;
    
public:
    
    bool joinByGap(InSequences &inSequences) { // joins two sequences via a gap based on instruction in gfa format
        
        return true;
        
    }
    
};

#endif /* gfastats-commons_h */
