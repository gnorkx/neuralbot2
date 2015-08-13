#include "bot.h"
#include "../sensors/rgba_vision.h"
#include "../sensors/simple_eyes.h"
#include "../sensors/status_feedback.h"


extern vector<object*> gWorld;
extern vector<bot*> gNewBots;

const float bot::LIFE_REDUCTION = 0.004;
bot::bot(Coord Pos)
{
    Pos_ = Pos;
    Vel_ = {0,0};
    Direction_ = {1,0};
    init();
}
bot::bot(const bot &rhs)
{
    copy_bot(rhs);
    copy_nn(rhs);

 //   sensors_ = rhs.sensors_; //need to clone sensor objects..s.?
    sensors_.push_back(new rgba_vision);
    nnet_TrainIn_ = new fann_type*[nnet_TrainSteps_];
    nnet_TrainOut_ = new fann_type*[nnet_TrainSteps_];
    for(int i = 0; i<nnet_TrainSteps_; i++)
    {
        nnet_TrainIn_[i] = new fann_type[nnet_nInputs_];
        nnet_TrainOut_[i] = new fann_type[nnet_nOutputs_];
        for(int j= 0; j<nnet_nInputs_; j++)
            nnet_TrainIn_[i][j] = rhs.nnet_TrainIn_[i][j];
        for(int j = 0; j<nnet_nOutputs_; j++)
            nnet_TrainOut_[i][j] = rhs.nnet_TrainOut_[i][j];
    }


}

bot::bot(bot &&rhs)
{
    copy_bot(rhs);

    nnet_ = rhs.nnet_;
    rhs.nnet_ = nullptr;

    sensors_.swap(rhs.sensors_);

    nnet_TrainIn_ = rhs.nnet_TrainIn_;
    nnet_TrainOut_ = rhs.nnet_TrainOut_;
    rhs.nnet_TrainIn_ = nullptr;
    rhs.nnet_TrainOut_ = nullptr;

}

void bot::copy_bot(const bot &rhs)
{
    Pos_        = rhs.Pos_;
    Direction_  = rhs.Direction_;
    Vel_        = rhs.Vel_;
    c_          = rhs.c_;
    size_       = rhs.size_;
    active_     = rhs.active_;

    Life_       = rhs.Life_;
    MaxLife_    = rhs.MaxLife_;

    Lifetime_   = rhs.Lifetime_;

    nSensorOut_ = rhs.nSensorOut_;

    nnet_nInputs_ = rhs.nnet_nInputs_;

}

void bot::copy_nn(const bot &rhs)
{
    nnet_ = new neural_net;
    *nnet_ = *rhs.nnet_;
}

bot::~bot()
{

    //delete sensors:
    for(auto it = sensors_.begin(); it!= sensors_.end();++it)
        delete *it;

    delete nnet_;

    if(nnet_TrainIn_ && nnet_TrainOut_)
    {
        for(int i = 0; i<nnet_TrainSteps_; i++)
        {
            delete[] nnet_TrainIn_[i];
            delete[] nnet_TrainOut_[i];
            nnet_TrainIn_[i] = nullptr;
            nnet_TrainOut_[i] = nullptr;
        }
        delete[] nnet_TrainIn_;
        delete[] nnet_TrainOut_;

        nnet_TrainIn_ = nullptr;
        nnet_TrainOut_ = nullptr;
    }

}


void bot::update()
{

    if(!active_) return;


    int nVal =0;
    float *Values = nullptr;
    fann_type nnet_input[nSensorOut_];
    int iValTotal = 0;
    for(auto it = sensors_.begin(); it!= sensors_.end();++it)
    {
        (*it)->update(*this, nVal, Values);

        for(int i = 0; i<nVal; i++, iValTotal)
        {
            nnet_input[iValTotal] = Values[i];
        }
    }
    fann_type *nnet_out = nnet_->run(nnet_input);

    //Vel_ = Coord(nnet_out[0], nnet_out[1]);

    actionM_.ChoseAction(*this, nnet_nOutputs_, nnet_out);
    if(Vel_.abs()>2)
        Vel_=2*Vel_.unit();
    //cout<<Vel_<< "\t // "<<Direction_<<endl;
    //Direction_ = Vel_;//Coord(nnet_out[2], nnet_out[3]).unit();
    Pos_+=0.75*Vel_;
    //Direction_ = Vel_.unit();
//    Direction_.rotate(0.005);



    //move this part into new class
    for(int i = std::min((unsigned)nnet_TrainSteps_,Lifetime_)-1; i>0; i--)
    {
        for(int j = 0; j<nnet_nInputs_; j++)
            nnet_TrainIn_[i][j] = nnet_TrainIn_[i-1][j];
        for(int j = 0; j<nnet_nOutputs_; j++)
            nnet_TrainOut_[i][j] = nnet_TrainOut_[i-1][j];
    }

    for(int j =0; j<nnet_nInputs_; j++)
        nnet_TrainIn_[0][j] = nnet_input[j];

    for(int j =0; j<nnet_nOutputs_; j++)
        nnet_TrainOut_[0][j] = nnet_out[j];


    Lifetime_++;


    object* TouchedFood = touch();
    if(TouchedFood)
    {
        eat((food*)TouchedFood);
       // learn();
//        bot* b = newChild();
//        delete b;
        gNewBots.push_back(newChild());
    }
    Life_ -= LIFE_REDUCTION;
    //cout<<"Life: "<<Life_<<endl;
    if(Life_<0)
    {
        kill();
        return;
    }
    if(Lifetime_ > 2*MaxLife_/LIFE_REDUCTION)
    {
        if(rnd()>exp(-Lifetime_ +  2*MaxLife_/LIFE_REDUCTION))
        {
            kill();
            return;
        }
    }


    renderer::Instance()->Circle(Pos_, size_, c_);

    Coord FOVA = Direction_; FOVA.rotate(-0.25*M_PI);
    Coord FOVB = Direction_; FOVB.rotate(0.25*M_PI);
   // cout<<FOVA<<"//////"<<FOVB<<endl;
    renderer::Instance()->Line(Pos_, 10*FOVA+Pos_,{200,200,0});
    renderer::Instance()->Line(Pos_, 10*FOVB+Pos_,{200,200,0});
//    renderer::Instance()->Line(Pos_, 10*(Pos_+Direction_-Direction_.perp()),{255,255,255});



}

object* bot::touch()
{
    //move this into sensor
   if(gWorld.size()>0)
    {
        std::partial_sort(gWorld.begin(),
            gWorld.begin()+1,
            gWorld.end(),
            [=]( object *a,  object *b){ float Diff_a = a->active_?(a->Pos_- Pos_).abs():9999.;
                                 float Diff_b = b->active_?(b->Pos_ - Pos_).abs():9999;
                                return Diff_a<Diff_b;}
                                );


        Coord Diff = gWorld[0]->Pos_ - Pos_;
        if(Diff.abs()<size_+gWorld[0]->size_ && gWorld[0]->active_)
            return gWorld[0];
    }

    return nullptr;
}

void bot::eat(food* food)
{
    if(!food) return;

    Life_ += food->life_;
    food->SetInactive();
}

void bot::learn()
{
    unsigned TrainSteps = std::min((unsigned) nnet_TrainSteps_,Lifetime_);
    if(TrainSteps > 0)
    {
        FANN::training_data tmp_tr;
        tmp_tr.set_train_data(TrainSteps,nnet_nInputs_,nnet_TrainIn_,nnet_nOutputs_,nnet_TrainOut_);
//        if(tr.length_train_data()>0)
//            tr.merge_train_data(tmp_tr);
//        else
        tmp_tr.set_train_data(TrainSteps,nnet_nInputs_,nnet_TrainIn_,nnet_nOutputs_,nnet_TrainOut_);
    }
}


void bot::init()
{

    c_ = {255,255,0};
    size_ = 1;
    Lifetime_ = 0;
    Life_ = MaxLife_ =1.;
    active_ = 1;
    sensors_.push_back(new simple_eyes);

    //sensors_.push_back(new status_feedback);
    //sensors_.push_back(new rgba_vision);

    nSensorOut_ = 0;
    for(auto it = sensors_.begin(); it!= sensors_.end(); ++it)
        nSensorOut_ += (*it)->nVal_;

    nnet_nInputs_ = nSensorOut_;
    nnet_= new neural_net;
    const unsigned neurons[] = {nSensorOut_,nnet_nOutputs_,nnet_nOutputs_};
    nnet_->create_standard_array(3,neurons);
    nnet_->set_activation_function_output(FANN::SIGMOID_SYMMETRIC);
    nnet_->set_activation_function_hidden(FANN::SIGMOID_SYMMETRIC);



    nnet_TrainIn_ = new fann_type*[nnet_TrainSteps_];
    nnet_TrainOut_ = new fann_type*[nnet_TrainSteps_];
    for(int i = 0; i<nnet_TrainSteps_; i++)
    {
        nnet_TrainIn_[i] = new fann_type[nnet_nInputs_];
        nnet_TrainOut_[i] = new fann_type[nnet_nOutputs_];
        for(int j= 0; j<nnet_nInputs_; j++)
            nnet_TrainIn_[i][j] = 0;
        for(int j = 0; j<nnet_nOutputs_; j++)
            nnet_TrainOut_[i][j] = 0;
    }

}

void bot::kill()
{
    active_ = 0;
    subject_.notify(this,stats::event::died);
    //gWorld.push_back(new food(Pos_,0.5*MaxLife_));
}

void bot::mutate()
{
   // MaxLife_ += MaxLife_*norm(0.05);
    //if(MaxLife_<0.3) MaxLife_ = 0.3;
    //if(Life_>MaxLife_) Life_ = MaxLife_;


    connection* weights = new connection[nnet_->get_total_connections()];
    nnet_->get_connection_array(weights);

    for (unsigned i = 0; i< nnet_->get_total_connections();i++)
    {
        weights[i].weight=weights[i].weight*(1+norm(0.9));
    }
    nnet_->set_weight_array(weights, nnet_->get_total_connections());

    delete[] weights;

}

bot* bot::newChild()
{
    bot *newBot = new bot(*this);

    newBot->Lifetime_ = 0;
    newBot->Life_ = MaxLife_;//Life_*1.1;
    newBot->mutate();
    subject_.notify(newBot,stats::event::born);
    return newBot;
}

float bot::GetFitness() const
{
    float fitness = 0;
    fitness += Lifetime_;
    fitness += Life_/LIFE_REDUCTION;
    return fitness;
}
