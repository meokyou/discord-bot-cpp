#include "fran.h"

int main(int argc, const char *argv[])
{
    dpp::cluster bot("token");

    const std::string log_name = "fran.log";

    dpp::commandhandler command_handler(&bot);

    command_hander.add_prefix(".").add_prefix("/");

    std::shared_ptr<spdlog::logger> log;
    spdlog::init_thread_pool(8192, 2);
    std::vector<spdlog::sink_ptr> sinks;
    auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_name, 1024 * 1024 * 5, 10);
    sinks.push_back(stdout_sink);
    sinks.push_back(rotating);
    log = std::make_shared<spdlog::async_logger>("logs", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(log);
    log->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%L] [th#%t] : %v");
    log->set_level(spdlog::level::level_enum::debug);

    bot.on_log([&bot, &log](const dpp::log_t & event) {
        switch (event.severity) {
            case dpp::ll_trace:
                log->trace("{}", event.message);
            break;
            case dpp::ll_debug:
                log->debug("{}", event.message);
            break;
            case dpp::ll_info:
                log->debug("{}", event.message);
            break;
            case dpp::ll_warning:
                log->debug("{}", event.message);
            break;
            case dpp::ll_error:
                log->debug("{}", event.message);
            break;
            case dpp::ll_critical:
            default:
                log->debug("{}", event.message);
            break;
            
        }
    });

    bot.on_ready[&bot](const dpp::ready_t & event) {

        std::cout << "Logged in as " << bot.me.username << "!\n";

        command_handler.add_command(

            "testing",

            {
                {"testparameter", dpp::param_info(dpp::pt_string, true, "Optional test parameter") }
            },

            [&command_handler](const std::string& command, const dpp::parameter_list_t& parameters,dpp::command_source src) {
                std::string got_param;
                if (!parameters.empty()) {
                    got_param = std::get<std::string>(parameters[0].second);
                }
                command_handler.reply(dpp::message("Ping --> " + got_param), src);
            },

            "Test server ping"
        );

        command_handler.add_command(

            "developer_info",

            {
                {"testparameter", dpp::param_info(dpp::pt_string, true, "Optional test parameter") }
            },

            [&command_hander](const std::string& command, const dpp::parameter_list_t& parameters, dpp::command_source src) {
                dpp::embed embed = dpp::embed().
                    set_color(0x0099ff).
                    set_title("Some name").
                    set_url("https://dpp.dev/").
                    set_author("Some name", "https://dpp.dev/", "https://dpp.dev/DPP-Logo.png").
                    set_description("Some description here").
                    set_thumbnail("https://dpp.dev/DPP-Logo.png").
                    add_field(
                        "Regular field title",
                        "Some value here"
                    ).
                    add_field(
                        "Inline field title",
                        "Some value here",
                        true
                    ).
                    add_field(
                        "Inline field title",
                        "Some value here",
                        true
                    ).
                    set_image("https://dpp.dev/DPP-Logo.png").
                    set_footer(dpp::embed_footer().set_text("Some footer text here").set_icon("https://dpp.dev/DPP-Logo.png")).
                    set_timestamp(time(0));
                    
                command_handler.reply(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));
            },

            "Share embed of developer information"
        );

        command_handler.register_commands();
        
        bot.global_command_create(dpp::slashcommand().set_name("help")
            .set_description("Display a list of avaliable commands")
            .set_application_id(bot.me.id)
        );

    });

    bot.on_interaction_create([&bot](const dpp::interaction_create_t & event) {
        if (event.command.type == dpp::it_application_command) {
            dpp:command_interaction cmd_data = std::get<dpp::command_interaction>(event.command.data);

            if (cmd_data.name == "help") {

                event.reply(dpp::ir_channel_message_with_source, "To be determined.");
            }
        }
    });

    bot.on_log([&bot](const dpp::log_t & event) {
        std::cout << dpp::utility::loglevel(event.severity) << ": " << event.message << "\n";
    });

    bot.start(false);

    return 0;
}